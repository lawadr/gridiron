/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "OgreWidget.h"

class Scene;
class SceneMouseEvent;

class SceneView : public OgreWidget {
    Q_OBJECT

public:
    SceneView(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~SceneView();

    Scene* scene() const;
    void setScene(Scene* scene);

protected:
    virtual void keyPressEvent(QKeyEvent* keyEvent);
    virtual void keyReleaseEvent(QKeyEvent* keyEvent);

    virtual void mouseDoubleClickEvent(QMouseEvent* mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent* mouseEvent);
    virtual void mousePressEvent(QMouseEvent* mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent* mouseEvent);

    virtual void wheelEvent(QWheelEvent* wheelEvent);

    virtual void initialise();

private:
    Scene* mScene;
    Ogre::Viewport* mViewport;
    Ogre::Camera* mCamera;

    void fillMouseEvent(SceneMouseEvent& sceneMouseEvent, const QMouseEvent* mouseEvent) const;
};
