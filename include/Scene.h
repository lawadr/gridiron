/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtCore/qobject.h>

class SceneItem;
class SceneMouseEvent;

class QKeyEvent;
class QWheelEvent;

namespace Ogre {
    class SceneManager;
    class Camera;
    class Ray;
}

class Scene : public QObject {
    Q_OBJECT

public:
    Scene(QObject* parent = 0);
    ~Scene();

    Ogre::SceneManager* sceneManager() const;

    void update();

    void addItem(SceneItem* item);
    void removeItem(SceneItem* item);

    Ogre::Camera* createCamera();
    void destroyCamera(Ogre::Camera* camera);

    virtual bool event(QEvent* event);

signals:
    void updated();

protected:
    virtual void keyPressEvent(QKeyEvent* keyEvent);
    virtual void keyReleaseEvent(QKeyEvent* keyEvent);

    virtual void mouseDoubleClickEvent(SceneMouseEvent* mouseEvent);
    virtual void mouseMoveEvent(SceneMouseEvent* mouseEvent);
    virtual void mousePressEvent(SceneMouseEvent* mouseEvent);
    virtual void mouseReleaseEvent(SceneMouseEvent* mouseEvent);

    virtual void wheelEvent(QWheelEvent* wheelEvent);

    virtual SceneItem* grabItem(const Ogre::Ray& ray) const;
    
private:
    Ogre::SceneManager* mSceneManager;
    QList<SceneItem*> mSceneItems;
    SceneItem* mFocusedSceneItem;
};
