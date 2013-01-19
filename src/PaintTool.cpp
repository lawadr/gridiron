/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "PaintTool.h"
#include "Map.h"
#include "Tile.h"
#include "MapGeometry.h"

PaintTool::PaintTool(Ogre::SceneManager* sceneManager, MapGeometry* mapGeometry)
    : Tool(sceneManager)
    , mSide(1)
    , mMapGeometry(mapGeometry)
    , mTile(0)
{
    mNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
    mRectangle = mSceneManager->createManualObject();
    mNode->attachObject(mRectangle);

    createRectangle();
    mNode->setVisible(false);
}

PaintTool::~PaintTool() {
    mSceneManager->destroyManualObject(mRectangle);
    mSceneManager->destroySceneNode(mNode);
}

Tile* PaintTool::tile() const {
    return mTile;
}

void PaintTool::setTile(Tile* tile) {
    if (mTile == tile)
        return;

    mTile = tile;

    if (mRectangle->getNumSections() > 0) {
        Ogre::MaterialManager::getSingleton().getByName(mTile->material())->load();
        mRectangle->getSection(0)->setMaterialName(mTile->material());
    }
}

//MOVE
int roundUp(float n) {
    n += 0.5f;
    return (n < 0.0f) ? n : n + 1.0f;
}

int roundDown(float n) {
    n += 0.5f;
    return (n < 0.0f) ? n - 1.0f : n;
}
//MOVE

bool PaintTool::mouseMoveEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    mNode->setVisible(false);

    Ogre::AxisAlignedBox levelBox(-0.5, mFloor * 2, -0.5, mMap->size().x() - 0.5, mFloor * 2 + 2, mMap->size().y() - 0.5);
    std::pair<bool, Ogre::Real> result = ray.intersects(levelBox);
    if (!result.first)
        return true;

    bool xIncrease = ray.getDirection().x >= 0.0f;
    bool yIncrease = ray.getDirection().z >= 0.0f;

    Ogre::Vector3 entry = ray.getPoint(result.second);

    bool found = false;
    Ogre::Real nearest = Ogre::Math::POS_INFINITY;
    bool eastWest = true;
    Position position;

    for (int i = (xIncrease) ? roundUp(entry.x) : roundDown(entry.x); (xIncrease) ? (i <= mMap->size().x()) : (i >= 0); (xIncrease) ? ++i : --i) {
        Ogre::Plane plane(Ogre::Vector3::UNIT_X, i - 0.5f);
        std::pair<bool, Ogre::Real> result = ray.intersects(plane);
        if (!result.first)
            continue;

        Ogre::Vector3 point = ray.getPoint(result.second);
        if ((point.y < (mFloor * 2.0f)) || (point.y > (mFloor * 2.0f + 2.0f))) break;

        int y = roundDown(point.z);
        if ((y < 0) || (y >= mMap->size().y())) break;
        if (mMap->hasWall(Position(i, y, mFloor), true)) {
            position = Position(i, y, mFloor);
            nearest = result.second;
            found = true;
            break;
        }
    }

    for (int j = (yIncrease) ? roundUp(entry.z) : roundDown(entry.z); (yIncrease) ? (j <= mMap->size().y()) : (j >= 0); (yIncrease) ? ++j : --j) {
        Ogre::Plane plane(Ogre::Vector3::UNIT_Z, j - 0.5f);
        std::pair<bool, Ogre::Real> result = ray.intersects(plane);
        if (!result.first)
            continue;

        if (result.second > nearest) break;
        Ogre::Vector3 point = ray.getPoint(result.second);
        if ((point.y < (mFloor * 2.0f)) || (point.y > (mFloor * 2.0f + 2.0f))) break;

        int x = roundDown(point.x);
        if ((x < 0) || (x >= mMap->size().x())) break;
        if (mMap->hasWall(Position(x, j, mFloor), false)) {
            position = Position(x, j, mFloor);
            eastWest = false;
            found = true;
            break;
        }
    }

    if (found) {
        mSide = (eastWest) ? ((xIncrease) ? 2 : 4) : ((yIncrease) ? 3 : 1);
        mPosition.setX((eastWest) ? ((xIncrease) ? position.x() - 1 : position.x()) : position.x());
        mPosition.setY((eastWest) ? position.y() : ((yIncrease) ? position.y() - 1 : position.y()));

        mNode->setPosition(mPosition.x(), mFloor * 2.0f, mPosition.y());

        Ogre::Degree angle((4 - (mSide - 1)) * 90.0f);
        mNode->setOrientation(Ogre::Quaternion(Ogre::Radian(angle), Ogre::Vector3::UNIT_Y));

        mNode->setVisible(true);
    }

    return true;
}

bool PaintTool::mouseReleaseEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    if (mouseEvent->button() != Qt::LeftButton)
        return false;

    if (!mTile)
        return false;

    Position position(mPosition.x(), mPosition.y(), mFloor);
    Cell cell = mMap->cell(position);
    cell.setTile(mSide, mTile);
    mMap->setCell(position, cell);

    mMapGeometry->update(mFloor);
    return true;
}

void PaintTool::createRectangle() {
    if (mRectangle->getNumSections() > 0)
        mRectangle->beginUpdate(0);
    else {
        Ogre::String materialName = (mTile) ? mTile->material() : "BaseWhite";
        Ogre::MaterialManager::getSingleton().getByName(materialName)->load();
        mRectangle->begin(materialName, Ogre::RenderOperation::OT_TRIANGLE_LIST);
    }

    mRectangle->position(-0.5f, 0.0f, -0.399f);
    mRectangle->normal(Ogre::Vector3::UNIT_Z);
    mRectangle->textureCoord(0.0f, 1.0f);

    mRectangle->position(0.5f, 0.0f, -0.399f);
    mRectangle->normal(Ogre::Vector3::UNIT_Z);
    mRectangle->textureCoord(1.0f, 1.0f);

    mRectangle->position(0.5f, 2.0f, -0.399f);
    mRectangle->normal(Ogre::Vector3::UNIT_Z);
    mRectangle->textureCoord(1.0f, 0.0f);

    mRectangle->position(-0.5f, 2.0f, -0.399f);
    mRectangle->normal(Ogre::Vector3::UNIT_Z);
    mRectangle->textureCoord(0.0f, 0.0f);

    mRectangle->quad(0, 1, 2, 3);

    mRectangle->end();
}
