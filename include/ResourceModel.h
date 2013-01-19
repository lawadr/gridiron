/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtCore/qabstractitemmodel.h>
#include <OGRE/OgreString.h>
#include <OGRE/OgreStringVector.h>

namespace Ogre {
    class ResourceManager;
}

class ResourceModel : public QAbstractListModel {
    Q_OBJECT

public:
    ResourceModel(QObject* parent = 0);
    ~ResourceModel();

    Ogre::ResourceManager* resourceManager() const;
    void setResourceManager(Ogre::ResourceManager* resourceManager);

    QString resourceGroup() const;
    void setResourceGroup(QString resourceGroup);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    Ogre::ResourceManager* mResourceManager;
    Ogre::String mResourceGroup;
};

class ResourceGroupModel : public QAbstractListModel {
    Q_OBJECT

public:
    ResourceGroupModel(QObject* parent = 0);
    ~ResourceGroupModel();

    QString resourceGroup() const;
    void setResourceGroup(QString resourceGroup);

    QString pattern() const;
    void setPattern(QString pattern);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    Ogre::String mResourceGroup;
    Ogre::String mPattern;
    Ogre::StringVectorPtr mResources;

    void fetchResources();
};