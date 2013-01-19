/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "WallTool.h"
#include "Map.h"
#include "Tile.h"
#include "MapGeometry.h"

WallTool::WallTool(Ogre::SceneManager* sceneManager, MapGeometry* mapGeometry)
    : Tool(sceneManager)
    , mMapGeometry(mapGeometry)
    , mSelecting(false)
    , mStart(0, 0)
    , mCurrent(0, 0)
    , mTile(0)
{
    mNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
    mWall = mSceneManager->createManualObject();
    mNode->attachObject(mWall);
}

WallTool::~WallTool() {
    mSceneManager->destroyManualObject(mWall);
    mSceneManager->destroySceneNode(mNode);
}

Tile* WallTool::tile() const {
    return mTile;
}

void WallTool::setTile(Tile* tile) {
    mTile = tile;
}

bool WallTool::mousePressEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    if ((mTile) && (mouseEvent->button() == Qt::LeftButton)) {
        mSelecting = true;
        mStart = calculatePosition(ray);
        mCurrent = mStart;
    }
    return false;
}

bool WallTool::mouseMoveEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    QPoint newPosition = calculatePosition(ray);

    if (newPosition == mCurrent)
        return false;

    mCurrent = newPosition;

    if (!mSelecting)
        return false;

    mNode->setPosition(0.0f, mFloor * 2.0f, 0.0f);

    int xDiff = abs(mStart.x() - mCurrent.x());
    int yDiff = abs(mStart.y() - mCurrent.y());

    if ((xDiff == 0) && (yDiff == 0)) {
        mWall->clear();
        return true;
    }

    int minX = (xDiff < yDiff) ? mStart.x() : std::min(mStart.x(), mCurrent.x());
    int minY = (xDiff < yDiff) ? std::min(mStart.y(), mCurrent.y()) : mStart.y();

    int maxX = (xDiff < yDiff) ? mStart.x() : std::max(mStart.x(), mCurrent.x());
    int maxY = (xDiff < yDiff) ? std::max(mStart.y(), mCurrent.y()) : mStart.y();

    if (mWall->getNumSections() > 0)
        mWall->beginUpdate(0);
    else {
        Ogre::MaterialManager::getSingleton().getByName(mTile->material())->load();
        mWall->begin(mTile->material(), Ogre::RenderOperation::OT_TRIANGLE_LIST);
    }

    if (xDiff < yDiff) {
        mWall->position(minX - 0.6f, 2.0f, minY - 0.6f);
        mWall->normal(-1.0f, 0.0f, 0.0f);
        mWall->textureCoord(0.0f, 0.0f);

        mWall->position(minX - 0.6f, 2.0f, maxY - 0.4f);
        mWall->normal(-1.0f, 0.0f, 0.0f);
        mWall->textureCoord(maxY - minY, 0.0f);

        mWall->position(minX - 0.6f, 0.0f, maxY - 0.4f);
        mWall->normal(-1.0f, 0.0f, 0.0f);
        mWall->textureCoord(maxY - minY, 1.0f);

        mWall->position(minX - 0.6f, 0.0f, minY - 0.6f);
        mWall->normal(-1.0f, 0.0f, 0.0f);
        mWall->textureCoord(0.0f, 1.0f);



        mWall->position(minX - 0.4f, 2.0f, maxY - 0.4f);
        mWall->normal(1.0f, 0.0f, 0.0f);
        mWall->textureCoord(0.0f, 0.0f);

        mWall->position(minX - 0.4f, 2.0f, minY - 0.6f);
        mWall->normal(1.0f, 0.0f, 0.0f);
        mWall->textureCoord(maxY - minY, 0.0f);

        mWall->position(minX - 0.4f, 0.0f, minY - 0.6f);
        mWall->normal(1.0f, 0.0f, 0.0f);
        mWall->textureCoord(maxY - minY, 1.0f);

        mWall->position(minX - 0.4f, 0.0f, maxY - 0.4f);
        mWall->normal(1.0f, 0.0f, 0.0f);
        mWall->textureCoord(0.0f, 1.0f);
    } else {
        mWall->position(maxX - 0.4f, 2.0f, minY - 0.6f);
        mWall->normal(0.0f, 0.0f, -1.0f);
        mWall->textureCoord(0.0f, 0.0f);

        mWall->position(minX - 0.6f, 2.0f, minY - 0.6f);
        mWall->normal(0.0f, 0.0f, -1.0f);
        mWall->textureCoord(maxX - minX, 0.0f);

        mWall->position(minX - 0.6f, 0.0f, minY - 0.6f);
        mWall->normal(0.0f, 0.0f, -1.0f);
        mWall->textureCoord(maxX - minX, 1.0f);

        mWall->position(maxX - 0.4f, 0.0f, minY - 0.6f);
        mWall->normal(0.0f, 0.0f, -1.0f);
        mWall->textureCoord(0.0f, 1.0f);



        mWall->position(minX - 0.6f, 2.0f, minY - 0.4f);
        mWall->normal(0.0f, 0.0f, 1.0f);
        mWall->textureCoord(0.0f, 0.0f);

        mWall->position(maxX - 0.4f, 2.0f, minY - 0.4f);
        mWall->normal(0.0f, 0.0f, 1.0f);
        mWall->textureCoord(maxX - minX, 0.0f);

        mWall->position(maxX - 0.4f, 0.0f, minY - 0.4f);
        mWall->normal(0.0f, 0.0f, 1.0f);
        mWall->textureCoord(maxX - minX, 1.0f);

        mWall->position(minX - 0.6f, 0.0f, minY - 0.4f);
        mWall->normal(0.0f, 0.0f, 1.0f);
        mWall->textureCoord(0.0f, 1.0f);
    }

    mWall->quad(0, 3, 2, 1);
    mWall->quad(4, 7, 6, 5);

    mWall->end();

    if (mWall->getNumSections() > 1)
        mWall->beginUpdate(1);
    else {
        //Ogre::MaterialManager::getSingleton().getByName("MasonryFloor")->load();
        //mWall->begin("MasonryFloor", Ogre::RenderOperation::OT_TRIANGLE_LIST);
        mWall->begin("UniformBlack", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    }

    mWall->position(minX - 0.6f, 2.0f, minY - 0.6f);
    mWall->normal(0.0f, 1.0f, 0.0f);
    mWall->textureCoord(-0.1f, -0.1f);

    mWall->position(maxX - 0.4f, 2.0f, minY - 0.6f);
    mWall->normal(0.0f, 1.0f, 0.0f);
    mWall->textureCoord((maxX - minX) + 0.1f, -0.1f);

    mWall->position(maxX - 0.4f, 2.0f, maxY - 0.4f);
    mWall->normal(0.0f, 1.0f, 0.0f);
    mWall->textureCoord((maxX - minX) + 0.1f, (maxY - minY) + 0.1f);

    mWall->position(minX - 0.6f, 2.0f, maxY - 0.4f);
    mWall->normal(0.0f, 1.0f, 0.0f);
    mWall->textureCoord(-0.1f, (maxY - minY) + 0.1f);

    mWall->quad(0, 3, 2, 1);

    if (xDiff < yDiff) {
        mWall->position(minX - 0.4f, 2.0f, minY - 0.6f);
        mWall->normal(0.0f, 0.0f, -1.0f);
        mWall->textureCoord(-0.1f, 0.0f);

        mWall->position(minX - 0.6f, 2.0f, minY - 0.6f);
        mWall->normal(0.0f, 0.0f, -1.0f);
        mWall->textureCoord(0.1f, 0.0f);

        mWall->position(minX - 0.6f, 0.0f, minY - 0.6f);
        mWall->normal(0.0f, 0.0f, -1.0f);
        mWall->textureCoord(0.1f, 2.0f);

        mWall->position(minX - 0.4f, 0.0f, minY - 0.6f);
        mWall->normal(0.0f, 0.0f, -1.0f);
        mWall->textureCoord(-0.1f, 2.0f);



        mWall->position(minX - 0.6f, 2.0f, maxY - 0.4f);
        mWall->normal(0.0f, 0.0f, 1.0f);
        mWall->textureCoord(-0.1f, 0.0f);

        mWall->position(minX - 0.4f, 2.0f, maxY - 0.4f);
        mWall->normal(0.0f, 0.0f, 1.0f);
        mWall->textureCoord(0.1f, 0.0f);

        mWall->position(minX - 0.4f, 0.0f, maxY - 0.4f);
        mWall->normal(0.0f, 0.0f, 1.0f);
        mWall->textureCoord(0.1f, 2.0f);

        mWall->position(minX - 0.6f, 0.0f, maxY - 0.4f);
        mWall->normal(0.0f, 0.0f, 1.0f);
        mWall->textureCoord(-0.1f, 2.0f);
    } else {
        mWall->position(minX - 0.6f, 2.0f, minY - 0.6f);
        mWall->normal(-1.0f, 0.0f, 0.0f);
        mWall->textureCoord(-0.1f, 0.0f);

        mWall->position(minX - 0.6f, 2.0f, minY - 0.4f);
        mWall->normal(-1.0f, 0.0f, 0.0f);
        mWall->textureCoord(0.1f, 0.0f);

        mWall->position(minX - 0.6f, 0.0f, minY - 0.4f);
        mWall->normal(-1.0f, 0.0f, 0.0f);
        mWall->textureCoord(0.1f, 2.0f);

        mWall->position(minX - 0.6f, 0.0f, minY - 0.6f);
        mWall->normal(-1.0f, 0.0f, 0.0f);
        mWall->textureCoord(-0.1f, 2.0f);



        mWall->position(maxX - 0.4f, 2.0f, minY - 0.4f);
        mWall->normal(1.0f, 0.0f, 0.0f);
        mWall->textureCoord(-0.1f, 0.0f);

        mWall->position(maxX - 0.4f, 2.0f, minY - 0.6f);
        mWall->normal(1.0f, 0.0f, 0.0f);
        mWall->textureCoord(0.1f, 0.0f);

        mWall->position(maxX - 0.4f, 0.0f, minY - 0.6f);
        mWall->normal(1.0f, 0.0f, 0.0f);
        mWall->textureCoord(0.1f, 2.0f);

        mWall->position(maxX - 0.4f, 0.0f, minY - 0.4f);
        mWall->normal(1.0f, 0.0f, 0.0f);
        mWall->textureCoord(-0.1f, 2.0f);
    }

    mWall->quad(4, 7, 6, 5);
    mWall->quad(8, 11, 10, 9);

    mWall->end();

    return true;
}

bool WallTool::mouseReleaseEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    if ((mouseEvent->button() != Qt::LeftButton) || !mSelecting) {
        mouseEvent->accept();
        return false;
    }

    int sizeX = mMap->size().x();
    int sizeY = mMap->size().y();

    int xDiff = abs(mStart.x() - mCurrent.x());
    int yDiff = abs(mStart.y() - mCurrent.y());

    int minX = (xDiff < yDiff) ? mStart.x() : std::min(mStart.x(), mCurrent.x());
    int minY = (xDiff < yDiff) ? std::min(mStart.y(), mCurrent.y()) : mStart.y();

    int maxX = (xDiff < yDiff) ? mStart.x() : std::max(mStart.x(), mCurrent.x());
    int maxY = (xDiff < yDiff) ? std::max(mStart.y(), mCurrent.y()) : mStart.y();

    if (xDiff < yDiff) {
        for (int i = minY; i < maxY; ++i) {
            if ((i < 0) || (i >= sizeY))
                continue;

            if (((minX - 1) < sizeX) && ((minX - 1) >= 0)) {
                Cell cell = mMap->cell(Position(minX - 1, i, mFloor));
                if (!cell.tile(2))
                    cell.setTile(2, mTile);
                mMap->setCell(Position(minX - 1, i, mFloor), cell);
            }

            if ((minX < sizeX) && (minX >= 0)) {
                Cell cell = mMap->cell(Position(minX, i, mFloor));
                if (!cell.tile(4))
                    cell.setTile(4, mTile);
                mMap->setCell(Position(minX, i, mFloor), cell);
            }
        }
    } else {
        for (int i = minX; i < maxX; ++i) {
            if ((i < 0) || (i >= sizeX))
                continue;

            if (((minY - 1) < sizeY) && ((minY - 1) >= 0)) {
                Cell cell = mMap->cell(Position(i, minY - 1, mFloor));
                if (!cell.tile(3))
                    cell.setTile(3, mTile);
                mMap->setCell(Position(i, minY - 1, mFloor), cell);
            }

            if ((minY < sizeY) && (minY >= 0)) {
                Cell cell = mMap->cell(Position(i, minY, mFloor));
                if (!cell.tile(1))
                    cell.setTile(1, mTile);
                mMap->setCell(Position(i, minY, mFloor), cell);
            }
        }
    }

    mMapGeometry->update(mFloor);
    mWall->clear();
    mSelecting = false;

    return true;
}

QPoint WallTool::calculatePosition(const Ogre::Ray& ray) {
    std::pair<bool, Ogre::Real> result = ray.intersects(Ogre::Plane(Ogre::Vector3::UNIT_Y, mFloor * 2.0f));
    if (result.first) {
        Ogre::Vector3 point = ray.getPoint(result.second);

        QPoint position;
        position.setX((point.x < 0.0f) ? point.x : (point.x + 1.0f));
        position.setY((point.z < 0.0f) ? point.z : (point.z + 1.0f));
        return position;
    }
    return QPoint();
}