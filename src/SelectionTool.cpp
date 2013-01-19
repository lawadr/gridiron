/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "SelectionTool.h"
#include "Map.h"

SelectionTool::SelectionTool(Ogre::SceneManager* sceneManager, MapGeometry* mapGeometry)
    : Tool(sceneManager)
    , mMapGeometry(mapGeometry)
    , mMoving(false)
{
}

void SelectionTool::activate() {
}

void SelectionTool::deactivate() {
}

bool SelectionTool::mousePressEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    if (mouseEvent->button() == Qt::LeftButton) {
        std::pair<bool, Ogre::Real> result = ray.intersects(Ogre::Plane(Ogre::Vector3::UNIT_Y, mFloor * 2.0f));
        if (!result.first)
            return false;

        Ogre::Vector3 point = ray.getPoint(result.second);

        QPoint position;
        position.setX((point.x < 0.0f) ? (point.x - 0.5f) : (point.x + 0.5f));
        position.setY((point.z < 0.0f) ? (point.z - 0.5f) : (point.z + 0.5f));

        const Cell& cell = mMap->cell(Position(position.x(), position.y(), mFloor));
        Object* object = cell.object();
        if (object) {
            mSelection.insert(object);

        }
        return true;
    }
    return false;
}

bool SelectionTool::mouseMoveEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    if (mMoving) {

    }
    return false;
}

bool SelectionTool::mouseReleaseEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
    return false;
}