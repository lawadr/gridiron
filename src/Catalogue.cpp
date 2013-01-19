/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Catalogue.h"
#include "Category.h"
#include "TileSet.h"

Catalogue::Catalogue() {
    mRoot = new Category;

    mTileSet = new TileSet;
    mTypeSet = new TypeSet;

    mRoot->addChild(mTileSet->root());
    mRoot->addChild(mTypeSet->root());
}

Catalogue::~Catalogue() {
    delete mTileSet;
    delete mTypeSet;

    delete mRoot;
}
