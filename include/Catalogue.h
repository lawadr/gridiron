/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

class Category;
class TileSet;
class TypeSet;

class Catalogue {
public:
    Catalogue();
    ~Catalogue();

    Category* root() const {
        return mRoot;
    }

    TileSet* tileSet() const {
        return mTileSet;
    }

    TypeSet* typeSet() const {
        return mTypeSet;
    }

private:
    Category* mRoot;
    TileSet* mTileSet;
    TypeSet* mTypeSet;
};
