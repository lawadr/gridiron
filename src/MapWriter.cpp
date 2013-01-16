/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "MapWriter.h"
#include "Map.h"
#include "TileSet.h"
#include "Tile.h"
#include <QtCore/qfile.h>
#include <QtXml/qxmlstream.h>

MapWriter::MapWriter() {
}

MapWriter::~MapWriter() {
}

bool MapWriter::write(const Map* map, QString path) {
	QFile file(path);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QXmlStreamWriter writer(&file);
	writer.writeStartDocument();

	writer.writeStartElement("map");
	writer.writeAttribute("xlength", QString::number(map->size().x()));
	writer.writeAttribute("ylength", QString::number(map->size().y()));
	writer.writeAttribute("zlength", QString::number(map->size().z()));

	writeProperties(map->properties(), writer);
	writeTileSet(*map->tileSet(), writer);
	writeCells(map, writer);

	writer.writeEndElement();

	writer.writeEndDocument();

	return true;
}

void MapWriter::writeProperty(const Property& prop, QXmlStreamWriter& writer) {
	writer.writeEmptyElement("property");
	writer.writeAttribute("key", prop.key());
	writer.writeAttribute("value", prop.value());
}

void MapWriter::writeProperties(const PropertyList& properties, QXmlStreamWriter& writer) {
	if (properties.empty())
		return;

	writer.writeStartElement("properties");

	for (PropertyList::const_iterator it = properties.begin(); it < properties.end(); ++it)
		writeProperty(*it, writer);

	writer.writeEndElement();
}

void MapWriter::writeTile(const Tile& tile, QXmlStreamWriter& writer) {
	writer.writeStartElement("tile");
	writer.writeAttribute("name", tile.name());
	writer.writeTextElement("material", QString::fromStdString(tile.material()));
	writeProperties(tile.properties(), writer);
	writer.writeEndElement();
}

void MapWriter::writeTileSet(const TileSet& tileSet, QXmlStreamWriter& writer) {
	writer.writeStartElement("tileset");

	QList<Tile*> tiles = tileSet.tiles();
	for (QList<Tile*>::const_iterator it = tiles.begin(); it < tiles.end(); ++it)
		writeTile(**it, writer);

	writer.writeEndElement();
}

void MapWriter::writeCells(const Map* map, QXmlStreamWriter& writer) {
	writer.writeStartElement("cells");

	QByteArray cellData;
	cellData.reserve(map->size().x() * map->size().y() * map->size().z() * 4);

	for (int z = 0; z < map->size().z(); ++z) { 
		for (int y = 0; y < map->size().y(); ++y) {
			for (int x = 0; x < map->size().x(); ++x) {
				const Cell& cell = map->cell(Position(x, y, z));
				
				for (int n = 0; n < 5; ++n) {
					Tile* tile = cell.tile(n);
					int id = tile ? tile->id() : 0;
					cellData.append(static_cast<char>(id));
					cellData.append(static_cast<char>(id >> 8));
					cellData.append(static_cast<char>(id >> 16));
					cellData.append(static_cast<char>(id >> 24));
				}
			}
		}
	}

	cellData = cellData.toBase64();
	writer.writeCharacters(QString::fromLatin1(cellData));

	writer.writeEndElement();
}