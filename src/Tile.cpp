/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Tile.h"

Tile::Tile(int id, Category* category)
    : mId(id)
    //, mCategory(category)
{
}

Tile::~Tile() {
}

/*Category* Tile::category() const {
return mCategory;
}*/

const std::string& Tile::material() const {
    return mMaterial;
}

void Tile::setMaterial(const std::string& material) {
    mMaterial = material;
}