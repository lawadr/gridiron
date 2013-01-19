/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "SceneView.h"
#include "Scene.h"
#include "SceneEvent.h"

SceneView::SceneView(QWidget* parent, Qt::WindowFlags flags)
    : OgreWidget(parent, flags)
    , mScene(0)
{
}

SceneView::~SceneView() {
}

Scene* SceneView::scene() const {
    return mScene;
}

void SceneView::setScene(Scene* scene) {
    if (mScene == scene)
        return;

    if (mScene) {
        disconnect(mScene, SIGNAL(changed()), this, SLOT(update()));
    }

    if (scene) {
        connect(scene, SIGNAL(changed()), this, SLOT(update()));
    }

    mScene = scene;
}

void SceneView::keyPressEvent(QKeyEvent* keyEvent) {
    if (mScene)
        mScene->keyPressEvent(keyEvent);
}

void SceneView::keyReleaseEvent(QKeyEvent* keyEvent) {
    if (mScene)
        mScene->keyReleaseEvent(keyEvent);
}

void SceneView::mouseMoveEvent(QMouseEvent* mouseEvent) {
    if (mScene) {
        SceneMouseEvent* sceneMouseEvent = new SceneMouseEvent(SceneMouseEvent::RELEASE_TYPE);
        fillMouseEvent(mouseEvent, sceneMouseEvent);
        mScene->mouseMoveEvent(sceneMouseEvent);
    }
}

void SceneView::mousePressEvent(QMouseEvent* mouseEvent) {
    if (mScene) {
        SceneMouseEvent* sceneMouseEvent = new SceneMouseEvent(SceneMouseEvent::RELEASE_TYPE);
        fillMouseEvent(mouseEvent, sceneMouseEvent);
        mScene->mousePressEvent(sceneMouseEvent);
    }
}

void SceneView::mouseReleaseEvent(QMouseEvent* mouseEvent) {
    if (mScene) {
        SceneMouseEvent* sceneMouseEvent = new SceneMouseEvent(SceneMouseEvent::RELEASE_TYPE);
        fillMouseEvent(mouseEvent, sceneMouseEvent);
        mScene->mouseReleaseEvent(sceneMouseEvent);
    }
}

void SceneView::fillMouseEvent(const QMouseEvent* mouseEvent, SceneMouseEvent* sceneMouseEvent) const {
    sceneMouseEvent->setButton(mouseEvent->button());
    sceneMouseEvent->setButtons(mouseEvent->buttons());
    sceneMouseEvent->setRay(mCamera->getCameraToViewportRay(mouseEvent->x() / width(), mouseEvent->y() / height()));
}
