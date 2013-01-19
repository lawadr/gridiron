/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Property.h"
#include <QtCore/qstring.h>

class Map;
class TileSet;
class Tile;
class QXmlStreamWriter;

class MapWriter {
public:
    MapWriter();
    ~MapWriter();

    bool write(const Map* map, QString path);

private:
    void writeProperty(const Property& prop, QXmlStreamWriter& writer);
    void writeProperties(const PropertyList& properties, QXmlStreamWriter& writer);

    void writeTile(const Tile& tile, QXmlStreamWriter& writer);
    void writeTileSet(const TileSet& tileSet, QXmlStreamWriter& writer);

    void writeCells(const Map* map, QXmlStreamWriter& writer);
};
