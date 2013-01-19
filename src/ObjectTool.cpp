/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "ObjectTool.h"
#include "Map.h"
#include "Type.h"
#include "MapGeometry.h"

ObjectTool::ObjectTool(Ogre::SceneManager* sceneManager, MapGeometry* mapGeometry)
    : Tool(sceneManager)
    , mMapGeometry(mapGeometry)
    , mType(0)
    , mObject(0)
{
    mNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    mNode2 = mNode->createChildSceneNode();
    mNode->setVisible(false);
}

ObjectTool::~ObjectTool() {
    if (mObject)
        mSceneManager->destroyEntity(mObject);
    mSceneManager->destroySceneNode(mNode2);
    mSceneManager->destroySceneNode(mNode);
}

Type* ObjectTool::type() const {
    return mType;
}

void ObjectTool::setType(Type* type) {
    if (mType == type)
        return;

    mType = type;

    if (mObject) {
        mSceneManager->destroyEntity(mObject);
        mObject = 0;
    }

    if (mType) {
        mObject = mSceneManager->createEntity(mType->mesh());
        mNode2->setScale(mType->scale(), mType->scale(), mType->scale());
        mNode2->setOrientation(Ogre::Quaternion(mType->rotation(), Ogre::Vector3::UNIT_Y));
        mNode2->setPosition(mType->offset());
        mNode2->attachObject(mObject);
    }
}

void ObjectTool::activate() {
    mNode->setVisible(true);
}

void ObjectTool::deactivate() {
    mNode->setVisible(false);
}

bool ObjectTool::mousePressEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    if (mouseEvent->button() == Qt::RightButton) {
        mAngle++;
        realign();
        return true;
    }
    return false;
}

bool ObjectTool::mouseMoveEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    std::pair<bool, Ogre::Real> result = ray.intersects(Ogre::Plane(Ogre::Vector3::UNIT_Y, mFloor * 2.0f));
    if (!result.first)
        return false;

    Ogre::Vector3 point = ray.getPoint(result.second);

    QPoint position;
    position.setX((point.x < 0.0f) ? (point.x - 0.5f) : (point.x + 0.5f));
    position.setY((point.z < 0.0f) ? (point.z - 0.5f) : (point.z + 0.5f));

    if (mPosition != position) {
        mPosition = position;
        realign();
        return true;
    }

    return true;
}

bool ObjectTool::mouseReleaseEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    if (mouseEvent->button() == Qt::LeftButton) {
        std::pair<bool, Ogre::Real> result = ray.intersects(Ogre::Plane(Ogre::Vector3::UNIT_Y, mFloor * 2.0f));
        if (result.first) {
            Ogre::Vector3 point = ray.getPoint(result.second);

            QPoint position;
            position.setX((point.x < 0.0f) ? (point.x - 0.5f) : (point.x + 0.5f));
            position.setY((point.z < 0.0f) ? (point.z - 0.5f) : (point.z + 0.5f));

            Object o(mType);
            o.setAngle(mAngle);
            o.setPosition(Position(position.x(), position.y(), mFloor));
            QRect bounds = o.boundingRectangle();
            if (collisionCheck(bounds))
                return false;

            Object* object = mMap->createObject(mType);
            object->setPosition(Position(position.x(), position.y(), mFloor));
            object->setAngle(mAngle);

            for (int j = bounds.top(); j <= bounds.top() + bounds.height(); ++j) {
                for (int i = bounds.left(); i <= bounds.left() + bounds.width(); ++i) {
                    Cell cell = mMap->cell(Position(i, j, mFloor));
                    cell.setObject(object);
                    mMap->setCell(Position(i, j, mFloor), cell);
                }
            }

            mMapGeometry->addObject(object);
            mNode->setVisible(false);
        }
    }
    return true;
}

void ObjectTool::realign() const {
    Object object(mType);
    object.setPosition(Position(mPosition.x(), mPosition.y(), mFloor));
    object.setAngle(mAngle);

    QRect rectangle = object.boundingRectangle();

    mNode->setVisible(!collisionCheck(rectangle));
    mNode->setPosition(rectangle.left() + rectangle.width() * 0.5f, mFloor * 2.0f, rectangle.top() + rectangle.height() * 0.5f);
    mNode->setOrientation(Ogre::Quaternion(-mAngle.toRadians(), Ogre::Vector3::UNIT_Y));
}

bool ObjectTool::collisionCheck(const QRect& bounds) const {
    bool result = isOutsideMap(bounds);
    result = result || floorCollisionCheck(bounds);
    result = result || wallCollisionCheck(bounds);
    result = result || objectCollisionCheck(bounds);
    return result;
}

bool ObjectTool::isOutsideMap(const QRect& bounds) const {
    return (bounds.left() < 0) || (bounds.top() < 0)
        || ((bounds.left() + bounds.width()) >= mMap->size().x())
        || ((bounds.top() + bounds.height()) >= mMap->size().y());
}

bool ObjectTool::floorCollisionCheck(const QRect& bounds) const {
    for (int j = bounds.top(); j <= bounds.top() + bounds.height(); ++j) {
        for (int i = bounds.left(); i <= bounds.left() + bounds.width(); ++i) {
            const Cell& cell = mMap->cell(Position(i, j, mFloor));
            if (!cell.tile(0))
                return true;
        }
    }

    return false;
}

bool ObjectTool::wallCollisionCheck(const QRect& bounds) const {
    for (int j = bounds.top(); j <= bounds.top() + bounds.height(); ++j) {
        for (int i = bounds.left() + 1; i <= bounds.left() + bounds.width(); ++i) {
            if (mMap->hasWall(Position(i, j, mFloor), true))
                return true;
        }
    }

    for (int j = bounds.top() + 1; j <= bounds.top() + bounds.height(); ++j) {
        for (int i = bounds.left(); i <= bounds.left() + bounds.width(); ++i) {
            if (mMap->hasWall(Position(i, j, mFloor), false))
                return true;
        }
    }

    return false;
}

bool ObjectTool::objectCollisionCheck(const QRect& bounds) const {
    for (int j = bounds.top(); j <= bounds.top() + bounds.height(); ++j) {
        for (int i = bounds.left(); i <= bounds.left() + bounds.width(); ++i) {
            const Cell& cell = mMap->cell(Position(i, j, mFloor));
            if (cell.object())
                return true;
        }
    }

    return false;
}