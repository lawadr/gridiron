/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "SceneItem.h"
#include "Object.h"
#include "Type.h"
#include "Grid.h"
#include <OgreSceneManager.h>
#include <OgreEntity.h>

/*SceneItem::SceneItem(Object* object, Ogre::SceneManager* sceneManager)
: mObject(object)
, mSceneManager(sceneManager)
, mNode(0)
, mEntity(0)
, mGrid(0)
{
mNode = sceneManager->getRootSceneNode()->createChildSceneNode();
mEntity = sceneManager->createEntity(object->type()->mesh());
mNode->attachObject(mEntity);


}*/

SceneItem::SceneItem()
    : mScene(0)
{
}

SceneItem::~SceneItem() {
}