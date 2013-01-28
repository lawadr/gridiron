/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

class Scene;
class SceneMouseEvent;

class QEvent;
class QKeyEvent;
class QWheelEvent;

namespace Ogre {
    class AxisAlignedBox;
}

class SceneItem {
public:
    friend class Scene;

    SceneItem();
    virtual ~SceneItem();

    Scene* scene() const {
        return mScene;
    }

    virtual Ogre::AxisAlignedBox& boundingBox() const = 0;

    virtual bool event(QEvent* event);

protected:
    Scene* mScene;

    virtual void keyPressEvent(QKeyEvent* keyEvent);
    virtual void keyReleaseEvent(QKeyEvent* keyEvent);

    virtual void mouseDoubleClickEvent(SceneMouseEvent* mouseEvent);
    virtual void mouseMoveEvent(SceneMouseEvent* mouseEvent);
    virtual void mousePressEvent(SceneMouseEvent* mouseEvent);
    virtual void mouseReleaseEvent(SceneMouseEvent* mouseEvent);

    virtual void wheelEvent(QWheelEvent* wheelEvent);

private:
    void setScene(Scene* scene) {
        mScene = scene;
    }
};
