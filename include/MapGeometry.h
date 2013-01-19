/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <OGRE/Ogre.h>
#include <QtCore/qlist.h>

class Map;
class Object;
class Region;

class MapGeometry {
public:
    MapGeometry(Ogre::SceneManager* sceneManager);
    ~MapGeometry();

    void setMap(Map* map);

    void setVisibility(float visibility);

    void update(int z);
    void updateAll();

    void addObject(Object* object);

private:
    struct Layer {
        Ogre::SceneNode* layerNode;
        Ogre::SceneNode* node;
        Region* region;
        QList<Ogre::Entity*> objects;
    };

    typedef std::vector<Layer> LayerList;

    Ogre::SceneManager* sceneManager_;
    Ogre::SceneNode* rootNode_;

    std::vector<Ogre::TexturePtr> mTextures;
    Ogre::TexturePtr mTileTexture;
    std::vector<Ogre::MaterialPtr> mMaterials;
    std::vector<Ogre::ManualObject*> mGeometry;

    Map* map_;

    LayerList layers_;

    void clear();
};
