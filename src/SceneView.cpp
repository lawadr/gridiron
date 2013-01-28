/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "SceneView.h"
#include "Scene.h"
#include "SceneEvent.h"

#include <QtGui/qapplication.h>

SceneView::SceneView(QWidget* parent, Qt::WindowFlags flags)
    : OgreWidget(parent, flags)
    , mScene(0)
    , mViewport(0)
    , mCamera(0)
{
    setMouseTracking(true);
}

SceneView::~SceneView() {
    if (mCamera)
        mScene->sceneManager()->destroyCamera(mCamera);
}

Scene* SceneView::scene() const {
    return mScene;
}

void SceneView::setScene(Scene* scene) {
    if (mScene == scene)
        return;

    if (mScene) {
        disconnect(mScene, SIGNAL(updated()), this, SLOT(update()));
        mScene->sceneManager()->destroyCamera(mCamera);
        mCamera = 0;
    }

    if (scene) {
        connect(scene, SIGNAL(updated()), this, SLOT(update()));
        mCamera = mScene->createCamera();
        if (mViewport)
            mViewport->setCamera(mCamera);
        else if (isInitialised())
            mViewport = renderWindow()->addViewport(mCamera);
    }

    mScene = scene;
}

void SceneView::keyPressEvent(QKeyEvent* keyEvent) {
    if (mScene)
        QApplication::sendEvent(mScene, keyEvent);
}

void SceneView::keyReleaseEvent(QKeyEvent* keyEvent) {
    if (mScene)
        QApplication::sendEvent(mScene, keyEvent);
}

void SceneView::mouseDoubleClickEvent(QMouseEvent* mouseEvent) {
    if (mScene) {
        SceneMouseEvent sceneMouseEvent(SceneMouseEvent::DOUBLE_CLICK_TYPE);
        fillMouseEvent(sceneMouseEvent, mouseEvent);
        QApplication::sendEvent(mScene, &sceneMouseEvent);
    }
}

void SceneView::mouseMoveEvent(QMouseEvent* mouseEvent) {
    if (mScene) {
        SceneMouseEvent sceneMouseEvent(SceneMouseEvent::MOVE_TYPE);
        fillMouseEvent(sceneMouseEvent, mouseEvent);
        QApplication::sendEvent(mScene, &sceneMouseEvent);
    }
}

void SceneView::mousePressEvent(QMouseEvent* mouseEvent) {
    if (mScene) {
        SceneMouseEvent sceneMouseEvent(SceneMouseEvent::PRESS_TYPE);
        fillMouseEvent(sceneMouseEvent, mouseEvent);
        QApplication::sendEvent(mScene, &sceneMouseEvent);
    }
}

void SceneView::mouseReleaseEvent(QMouseEvent* mouseEvent) {
    if (mScene) {
        SceneMouseEvent sceneMouseEvent(SceneMouseEvent::RELEASE_TYPE);
        fillMouseEvent(sceneMouseEvent, mouseEvent);
        QApplication::sendEvent(mScene, &sceneMouseEvent);
    }
}

void SceneView::wheelEvent(QWheelEvent* wheelEvent) {
    if (mScene)
        QApplication::sendEvent(mScene, wheelEvent);
}

void SceneView::initialise() {
    if (mCamera)
        mViewport = renderWindow()->addViewport(mCamera);
}

void SceneView::fillMouseEvent(SceneMouseEvent& sceneMouseEvent, const QMouseEvent* mouseEvent) const {
    sceneMouseEvent.setButton(mouseEvent->button());
    sceneMouseEvent.setButtons(mouseEvent->buttons());
    sceneMouseEvent.setRay(mCamera->getCameraToViewportRay(mouseEvent->x() / width(), mouseEvent->y() / height()));
}

