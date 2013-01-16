/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "ResourceModel.h"
#include <OGRE/OgreResourceManager.h>
#include <OGRE/OgreResourceGroupManager.h>

ResourceModel::ResourceModel(QObject* parent)
	: QAbstractListModel(parent)
	, mResourceManager(0)
	, mResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)
{
}

ResourceModel::~ResourceModel() {
}

Ogre::ResourceManager* ResourceModel::resourceManager() const {
	return mResourceManager;
}

void ResourceModel::setResourceManager(Ogre::ResourceManager* resourceManager) {
	if (mResourceManager != resourceManager) {
		beginResetModel();
		mResourceManager = resourceManager;
		endResetModel();
	}
}

QString ResourceModel::resourceGroup() const {
	return QString::fromStdString(mResourceGroup);
}

void ResourceModel::setResourceGroup(QString resourceGroup) {
	Ogre::String group = resourceGroup.toStdString();
	if (mResourceGroup != group) {
		beginResetModel();
		mResourceGroup = group;
		endResetModel();
	}
}

int ResourceModel::rowCount(const QModelIndex& parent) const {
	if (parent.isValid())
		return 0;

	Ogre::ResourceManager::ResourceMapIterator i = mResourceManager->getResourceIterator();
	int resourceCount = 0;
	while (i.hasMoreElements()) {
		if (i.getNext()->getGroup() == mResourceGroup)
			++resourceCount;
	}
	return resourceCount;
}

QVariant ResourceModel::data(const QModelIndex& index, int role) const {
	if (role == Qt::DisplayRole) {
		Ogre::ResourceManager::ResourceMapIterator i = mResourceManager->getResourceIterator();
		int resourceCount = 0;
		while (i.hasMoreElements()) {
			Ogre::ResourcePtr resource = i.getNext();
			if (resource->getGroup() == mResourceGroup) {
				if (resourceCount == index.row())
					return QString::fromStdString(resource->getName());
				++resourceCount;
			}
		}	
	}
	return QVariant();
}

//---------------------------------------------------------------------------------------

ResourceGroupModel::ResourceGroupModel(QObject* parent)
	: QAbstractListModel(parent)
	, mResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)
{
	fetchResources();
}

ResourceGroupModel::~ResourceGroupModel() {
}

QString ResourceGroupModel::resourceGroup() const {
	return QString::fromStdString(mResourceGroup);
}

void ResourceGroupModel::setResourceGroup(QString resourceGroup) {
	Ogre::String newResourceGroup = resourceGroup.toStdString();
	if (mResourceGroup != newResourceGroup) {
		beginResetModel();
		mResourceGroup = newResourceGroup;
		fetchResources();
		endResetModel();
	}
}

QString ResourceGroupModel::pattern() const {
	return QString::fromStdString(mPattern);
}

void ResourceGroupModel::setPattern(QString pattern) {
	Ogre::String newPattern = pattern.toStdString();
	if (mPattern != newPattern) {
		beginResetModel();
		mPattern = newPattern;
		fetchResources();
		endResetModel();
	}
}

int ResourceGroupModel::rowCount(const QModelIndex& parent) const {
	if (parent.isValid())
		return 0;

	return mResources->size();
}

QVariant ResourceGroupModel::data(const QModelIndex& index, int role) const {
	if (role == Qt::DisplayRole) {
		return QString::fromStdString(mResources->at(index.row()));
	}

	return QVariant();
}

void ResourceGroupModel::fetchResources() {
	Ogre::ResourceGroupManager* resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();

	if (mPattern.empty())
		mResources = resourceGroupManager->listResourceNames(mResourceGroup);
	else
		mResources = resourceGroupManager->findResourceNames(mResourceGroup, mPattern);
}