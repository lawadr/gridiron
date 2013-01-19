/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "MapReader.h"
#include "Map.h"
#include "TileSet.h"
#include "Tile.h"
#include <QtCore/qfile.h>
#include <QtXml/qxmlstream.h>

MapReader::MapReader() {
}

MapReader::~MapReader() {
}

Map* MapReader::readMap(QString path) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QXmlStreamReader reader(&file);

    if (reader.readNextStartElement()) {
        if (reader.name() == "map") {
            return readMap(reader);
        }
    }

    return 0;
}

Map* MapReader::readMap(QXmlStreamReader& reader) {
    QXmlStreamAttributes attributes = reader.attributes();

    Size size;
    size.setX(attributes.value("xlength").toString().toInt());
    size.setY(attributes.value("ylength").toString().toInt());
    size.setZ(attributes.value("zlength").toString().toInt());

    Map* map = new Map(size);

    while (reader.readNextStartElement()) {
        if (reader.name() == "properties")
            readProperties(reader);
        if (reader.name() == "tileset")
            readTileSet(map, reader);
        else if (reader.name() == "cells")
            readCells(map, reader);
        else
            reader.skipCurrentElement();
    }

    return map;
}

void MapReader::readProperty(PropertyList& properties, QXmlStreamReader& reader) {
    Property prop;

    QXmlStreamAttributes attributes = reader.attributes();
    prop.setKey(attributes.value("key").toString());
    prop.setValue(attributes.value("value").toString());

    properties.push_back(prop);
}

void MapReader::readProperties(QXmlStreamReader& reader) {
    PropertyList properties;

    while (reader.readNextStartElement()) {
        if (reader.name() == "property") {
            readProperty(properties, reader);
        } else
            reader.skipCurrentElement();
    }
}

void MapReader::readTile(Tile& tile, QXmlStreamReader& reader) {
    QXmlStreamAttributes attributes = reader.attributes();
    tile.setName(attributes.value("name").toString());

    while (reader.readNextStartElement()) {
        if (reader.name() == "material") {
            tile.setMaterial(reader.readElementText().toStdString());
        } else
            reader.skipCurrentElement();
    }
}

void MapReader::readTileSet(Map* map, QXmlStreamReader& reader) {
    TileSet* tileSet = map->tileSet();

    while (reader.readNextStartElement()) {
        if (reader.name() == "tile") {
            Tile* tile = static_cast<Tile*>(tileSet->createItem());
            readTile(*tile, reader);
        } else
            reader.skipCurrentElement();
    }
}

//TEMP
//#include <QtCore/qmap.h>
//TEMP
void MapReader::readCells(Map* map, QXmlStreamReader& reader) {
    reader.readNext();

    QByteArray cellData = QString::fromRawData(reader.text().unicode(), reader.text().size()).toLatin1();
    cellData = QByteArray::fromBase64(cellData);

    //TEMP
    //	QMap<int, Tile*> tileMap;
    //	Tile* t = tileMap.insert(1, new Tile(1)).value();
    //	t->setName("Default Floor");
    //	t->setMaterial("SlabFloor");
    //	t = tileMap.insert(2, new Tile(2)).value();
    //	t->setName("Default Wall");
    //	t->setMaterial("TileWall");
    //	t = tileMap.insert(3, new Tile(3)).value();
    //	t->setName("Default Paint");
    //	t->setMaterial("BrickWall");
    //TEMP

    int x = 0, y = 0, z = 0;
    for (int i = 0; i < (cellData.size() - 19); i += 20) {

        Cell cell;
        for (int n = 0; n < 5; ++n) {
            int index = i + n * 4;

            int id = cellData.at(index);
            id += cellData.at(index + 1) << 8;
            id += cellData.at(index + 2) << 16;
            id += cellData.at(index + 3) << 24;

            if (id) {
                //TEMP
                //Tile* tile = 0;
                //if (tileMap.contains(id))
                //	tile = tileMap[id];
                //else
                //	tile = tileMap.insert(id, new Tile(id)).value();
                //TEMP
                Tile * tile = map->tileSet()->tiles().at(id - 1);
                cell.setTile(n, tile);
            }
        }
        map->setCell(Position(x, y, z), cell);

        ++x;
        if (x >= map->size().x()) {
            x = 0;
            ++y;
            if (y >= map->size().y()) {
                y = 0;
                ++z;
                if (z >= map->size().z())
                    break;
            }
        }
    }

    reader.skipCurrentElement();
}