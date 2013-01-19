/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Property.h"
#include <QtCore/qstring.h>

class Map;
class TileSet;
class Tile;
class QXmlStreamReader;

class MapReader {
public:
    MapReader();
    ~MapReader();

    Map* readMap(QString path);

private:
    Map* readMap(QXmlStreamReader& reader);

    void readProperty(PropertyList& properties, QXmlStreamReader& reader);
    void readProperties(QXmlStreamReader& reader);

    void readTile(Tile& tile, QXmlStreamReader& reader);
    void readTileSet(Map* map, QXmlStreamReader& reader);

    void readCells(Map* map, QXmlStreamReader& reader);
};
