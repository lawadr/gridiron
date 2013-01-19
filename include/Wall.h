/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

//#include <QtCore/qvector.h>

class Tile;

class Wall {
public:
    Wall(int start, int end, Tile* mTile);

private:
    int mStart;
    int mEnd;

    Tile* mTile;
    //QVector<Tile*> mSide1;
    //QVector<Tile*> mSide2;
};
