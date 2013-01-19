/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Map.h"
#include "TileSet.h"

Map::Map(const Size& size)
    : mSize(size)
    , mCells(size.x() * size.y() * size.z())
    //, mWallsX(size.y() + 1)
    //, mWallsY(size.x() + 1)
{
    mCatalogue = new Catalogue;
}

Map::~Map() {
    QVector<Object*>::iterator i;
    for (i = mObjects.begin(); i != mObjects.end(); ++i)
        delete *i;

    delete mCatalogue;
}

Object* Map::createObject(Type* type) {
    Object* object = new Object(type);
    mObjects.push_back(object);
    return object;
}

void Map::destroyObject(Object* object) {
    int index = mObjects.indexOf(object);
    mObjects.remove(index);
    delete object;
}

Object* Map::object(int index) const {
    return mObjects.at(index);
}

int Map::objectCount() const {
    return mObjects.size();
}