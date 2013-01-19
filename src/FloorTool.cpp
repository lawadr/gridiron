/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "FloorTool.h"
#include "Map.h"
#include "Tile.h"
#include "MapGeometry.h"

FloorTool::FloorTool(Ogre::SceneManager* sceneManager, MapGeometry* mapGeometry)
    : Tool(sceneManager)
    , mMapGeometry(mapGeometry)
    , mSelecting(false)
    , mClearing(false)
    , mStart(0, 0)
    , mCurrent(0, 0)
    , mRectangle(0)
    , mNode(0)
    , mTile(0)
{
    mNode = mSceneManager->createSceneNode();
    mRectangle = mSceneManager->createManualObject();
    mNode->attachObject(mRectangle);

    Ogre::MaterialManager* materialManager = Ogre::MaterialManager::getSingletonPtr();
    Ogre::ResourcePtr resource = materialManager->create("FloorTool", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mMaterial = static_cast<Ogre::MaterialPtr>(resource);

    createRectangle();
}

FloorTool::~FloorTool() {
    mSceneManager->destroyManualObject(mRectangle);
    mSceneManager->destroySceneNode(mNode);
}

Tile* FloorTool::tile() const {
    return mTile;
}

void FloorTool::setTile(Tile* tile) {
    mTile = tile;

    Ogre::MaterialManager* manager = Ogre::MaterialManager::getSingletonPtr();
    Ogre::String name = tile->material();
    Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(manager->getByName(name));

    if (material.isNull())
        return;

    material->copyDetailsTo(mMaterial);
    mMaterial->setDepthBias(1, 0);
}

void FloorTool::activate() {
    mSceneManager->getRootSceneNode()->addChild(mNode);
}

void FloorTool::deactivate() {
    mSceneManager->getRootSceneNode()->removeChild(mNode);
}

bool FloorTool::mousePressEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    if (mSelecting || mouseEvent->button() != Qt::LeftButton && mouseEvent->button() != Qt::RightButton)
        return false;

    mClearing = mouseEvent->button() == Qt::RightButton;
    mSelecting = true;
    mStart = calculatePosition(ray);
    mCurrent = mStart;

    return false;
}

bool FloorTool::mouseMoveEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    QPoint newPosition = calculatePosition(ray);

    if (newPosition == mCurrent)
        return false;

    mCurrent = newPosition;

    mNode->setPosition(mCurrent.x(), mFloor * 2.0f, mCurrent.y());

    if (mSelecting)
        createRectangle();

    return true;
}

bool FloorTool::mouseReleaseEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    if (!mSelecting) {
        mouseEvent->accept();
        return false;
    }

    if (mClearing && (mouseEvent->button() != Qt::RightButton)) {
        mouseEvent->accept();
        return false;
    }

    if (!mClearing && (mouseEvent->button() != Qt::LeftButton)) {
        mouseEvent->accept();
        return false;
    }

    QPoint minimum(std::min(mStart.x(), mCurrent.x()), std::min(mStart.y(), mCurrent.y()));
    QPoint maximum(std::max(mStart.x(), mCurrent.x()), std::max(mStart.y(), mCurrent.y()));

    int sizeX = mMap->size().x();
    int sizeY = mMap->size().y();

    minimum.setX((minimum.x() < 0) ? 0 : minimum.x());
    minimum.setY((minimum.y() < 0) ? 0 : minimum.y());
    maximum.setX((maximum.x() >= sizeX) ? (sizeX - 1) : maximum.x());
    maximum.setY((maximum.y() >= sizeY) ? (sizeY - 1) : maximum.y());

    Tile* tile = (mClearing) ? 0 : mTile;
    for (int j = minimum.y(); j <= maximum.y(); ++j) {
        for (int i = minimum.x(); i <= maximum.x(); ++i) {
            Cell cell = mMap->cell(Position(i, j, mFloor));
            cell.setTile(0, tile);
            mMap->setCell(Position(i, j, mFloor), cell);
        }
    }

    mMapGeometry->update(mFloor);

    mStart = mCurrent;
    createRectangle();

    mSelecting = false;
    mClearing = false;

    return true;
}

QPoint FloorTool::calculatePosition(const Ogre::Ray& ray) {
    std::pair<bool, Ogre::Real> result = ray.intersects(Ogre::Plane(Ogre::Vector3::UNIT_Y, mFloor * 2.0f));
    if (result.first) {
        Ogre::Vector3 point = ray.getPoint(result.second);

        QPoint position;
        position.setX((point.x < 0.0f) ? (point.x - 0.5f) : (point.x + 0.5f));
        position.setY((point.z < 0.0f) ? (point.z - 0.5f) : (point.z + 0.5f));
        return position;
    }
    return QPoint();
}

void FloorTool::createRectangle() {
    if (mRectangle->getNumSections() > 0)
        mRectangle->beginUpdate(0);
    else {
        mRectangle->begin("FloorTool", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    }

    QPoint p = mStart - mCurrent;

    Ogre::Real minX = std::min(p.x(), 0) - 0.5f;
    Ogre::Real minY = std::min(p.y(), 0) - 0.5f;

    Ogre::Real maxX = std::max(p.x(), 0) + 0.5f;
    Ogre::Real maxY = std::max(p.y(), 0) + 0.5f;

    mRectangle->position(minX, 0.0f, minY);
    mRectangle->normal(0.0f, 1.0f, 0.0f);
    mRectangle->textureCoord(0.0f, 0.0f);

    mRectangle->position(maxX, 0.0f, minY);
    mRectangle->normal(0.0f, 1.0f, 0.0f);
    mRectangle->textureCoord(maxX - minX, 0.0f);

    mRectangle->position(maxX, 0.0f, maxY);
    mRectangle->normal(0.0f, 1.0f, 0.0f);
    mRectangle->textureCoord(maxX - minX, maxY - minY);

    mRectangle->position(minX, 0.0f, maxY);
    mRectangle->normal(0.0f, 1.0f, 0.0f);
    mRectangle->textureCoord(0.0f, maxY - minY);

    mRectangle->quad(3, 2, 1, 0);

    mRectangle->end();
}
