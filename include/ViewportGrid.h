/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <OGRE/Ogre.h>

class ViewportGrid : public Ogre::RenderTargetListener {
public:
    ViewportGrid(Ogre::SceneManager* sceneManager, Ogre::Viewport* viewport);
    ~ViewportGrid();

    Ogre::SceneManager* getSceneManager() const;
    Ogre::Viewport* getViewport() const;

    const Ogre::ColourValue& getColour() const;
    void setColour(const Ogre::ColourValue& colour);

    const Ogre::Vector3& getOrigin() const;
    void setOrigin(const Ogre::Vector3& origin);

    const Ogre::Quaternion& getOrientation() const;
    void setOrientation(const Ogre::Quaternion& orientation);

    bool getVisible() const;
    void setVisible(bool visible);

    void preViewportUpdate(const Ogre::RenderTargetViewportEvent& viewportEvent);
    void postViewportUpdate(const Ogre::RenderTargetViewportEvent& viewportEvent);

private:
    static const Ogre::String MATERIAL_NAME;

    Ogre::SceneManager* sceneManager_;
    Ogre::Viewport* viewport_;

    Ogre::SceneNode* orientationNode_;
    Ogre::SceneNode* translationNode_;

    Ogre::ManualObject* lines_;
    size_t size_;

    Ogre::Plane plane_;

    Ogre::ColourValue colour_;
    bool visible_;

    void resizeGeometry();
};
