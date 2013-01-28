/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Scene.h"
#include "SceneItem.h"
#include "SceneEvent.h"

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>

Scene::Scene(QObject* parent)
    : QObject(parent)
    , mFocusedSceneItem(0)
{
    mSceneManager = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC);
}

Scene::~Scene() {
    Ogre::Root::getSingletonPtr()->destroySceneManager(mSceneManager);
}

Ogre::SceneManager* Scene::sceneManager() const {
    return mSceneManager;
}

void Scene::update() {
    emit updated();
}

void Scene::addItem(SceneItem* item) {
    Scene* previous = item->scene();
    if (previous)
        previous->removeItem(item);

    item->setScene(this);
    mSceneItems.append(item);
}

void Scene::removeItem(SceneItem* item) {
    item->setScene(0);
    mSceneItems.removeOne(item);
    if (mFocusedSceneItem == 0)
        mFocusedSceneItem = 0;
}

Ogre::Camera* Scene::createCamera() {
    static Ogre::NameGenerator cameraNameGenerator("SceneCamera");
    Ogre::String name = cameraNameGenerator.generate();
    return mSceneManager->createCamera(name);
}

void Scene::destroyCamera(Ogre::Camera* camera) {
    mSceneManager->destroyCamera(camera);
}

bool Scene::event(QEvent* event) {
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

void Scene::keyPressEvent(QKeyEvent* keyEvent) {
    if (mFocusedSceneItem)
        mFocusedSceneItem->event(keyEvent);
}

void Scene::keyReleaseEvent(QKeyEvent* keyEvent) {
    if (mFocusedSceneItem)
        mFocusedSceneItem->event(keyEvent);
}

void Scene::mouseDoubleClickEvent(SceneMouseEvent* mouseEvent) {
    if (mFocusedSceneItem)
        mFocusedSceneItem->event(mouseEvent);
}

void Scene::mouseMoveEvent(SceneMouseEvent* mouseEvent) {
    mFocusedSceneItem = grabItem(mouseEvent->ray());
    if (mFocusedSceneItem)
        mFocusedSceneItem->event(mouseEvent);
}

void Scene::mousePressEvent(SceneMouseEvent* mouseEvent) {
    if (mFocusedSceneItem)
        mFocusedSceneItem->event(mouseEvent);
}

void Scene::mouseReleaseEvent(SceneMouseEvent* mouseEvent) {
    if (mFocusedSceneItem)
        mFocusedSceneItem->event(mouseEvent);
}

void Scene::wheelEvent(QWheelEvent* wheelEvent) {
    if (mFocusedSceneItem)
        mFocusedSceneItem->event(wheelEvent);
}

SceneItem* Scene::grabItem(const Ogre::Ray& ray) const {
    for (QList<SceneItem*>::const_iterator it = mSceneItems.begin(); it != mSceneItems.end(); ++it) {
        SceneItem* item = *it;
        std::pair<bool, Ogre::Real> result = ray.intersects(item->boundingBox());
        if (result.first)
            return item;
    }
    return 0;
}