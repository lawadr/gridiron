/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Scene.h"
#include "SceneItem.h"
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>

Scene::Scene(QObject* parent)
    : QObject(parent)
{
    mSceneManager = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC);
}

Scene::~Scene() {
    Ogre::Root::getSingletonPtr()->destroySceneManager(mSceneManager);
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
}

Ogre::SceneManager* Scene::sceneManager() const {
    return mSceneManager;
}

void Scene::keyPressEvent (QKeyEvent* keyEvent) {
}

void Scene::keyReleaseEvent (QKeyEvent* keyEvent) {
}

void Scene::mouseMoveEvent(SceneMouseEvent* mouseEvent) {
}

void Scene::mousePressEvent(SceneMouseEvent* mouseEvent) {
}

void Scene::mouseReleaseEvent(SceneMouseEvent* mouseEvent) {
}

void Scene::update() {
    emit changed();
}