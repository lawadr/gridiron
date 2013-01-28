/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "SceneItem.h"
#include "SceneEvent.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>

SceneItem::SceneItem()
    : mScene(0)
{
}

SceneItem::~SceneItem() {
}

bool SceneItem::event(QEvent* event) {
    switch (event->type()) {
    case QEvent::KeyPress:
        keyPressEvent(static_cast<QKeyEvent*>(event));
        break;
    case QEvent::KeyRelease:
        keyReleaseEvent(static_cast<QKeyEvent*>(event));
        break;
    case SceneMouseEvent::DOUBLE_CLICK_TYPE:
        mouseDoubleClickEvent(static_cast<SceneMouseEvent*>(event));
        break;
    case SceneMouseEvent::MOVE_TYPE:
        mouseMoveEvent(static_cast<SceneMouseEvent*>(event));
        break;
    case SceneMouseEvent::PRESS_TYPE:
        mousePressEvent(static_cast<SceneMouseEvent*>(event));
        break;
    case SceneMouseEvent::RELEASE_TYPE:
        mouseReleaseEvent(static_cast<SceneMouseEvent*>(event));
        break;
    case QEvent::Wheel:
        wheelEvent(static_cast<QWheelEvent*>(event));
        break;
    default:
        return false;
    }
    return true;
}

void SceneItem::keyPressEvent(QKeyEvent* keyEvent) {
}

void SceneItem::keyReleaseEvent(QKeyEvent* keyEvent) {
}

void SceneItem::mouseDoubleClickEvent(SceneMouseEvent* mouseEvent) {
}

void SceneItem::mouseMoveEvent(SceneMouseEvent* mouseEvent) {
}

void SceneItem::mousePressEvent(SceneMouseEvent* mouseEvent) {
}

void SceneItem::mouseReleaseEvent(SceneMouseEvent* mouseEvent) {
}

void SceneItem::wheelEvent(QWheelEvent* wheelEvent) {
}