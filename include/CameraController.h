/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <OGRE/Ogre.h>
#include <QtGui/qevent.h>
#include <QtCore/qbasictimer.h>

class MapView;

class AbstractCameraController : public QObject {
    Q_OBJECT

public:
    AbstractCameraController(QWidget* widget);
    ~AbstractCameraController();

    bool eventFilter(QObject* watched, QEvent* e);

protected:
    void update();

    virtual void injectKeyPressEvent(QKeyEvent* keyEvent) {}
    virtual void injectKeyReleaseEvent(QKeyEvent* keyEvent) {}

    virtual void injectMousePressEvent(QMouseEvent* mouseEvent) {}
    virtual void injectMouseMoveEvent(QMouseEvent* mouseEvent) {}

    virtual void injectWheelEvent(QWheelEvent* wheelEvent) {}

    virtual void injectTimerEvent(QTimerEvent* timerEvent) {}

private:
    QWidget* mWidget;
};

class CameraController : public AbstractCameraController {
public:
    CameraController(QWidget* widget, Ogre::SceneManager* sceneManager, Ogre::Camera* camera);
    ~CameraController();

    Ogre::SceneManager* getSceneManager() const;
    Ogre::Camera* getCamera() const;

    float getY() const;
    void setY(float y);

    void injectMousePressEvent(QMouseEvent* mouseEvent);
    void injectMouseMoveEvent(QMouseEvent* mouseEvent);

    void injectWheelEvent(QWheelEvent* wheelEvent);

    void injectKeyPressEvent(QKeyEvent* keyEvent);
    void injectKeyReleaseEvent(QKeyEvent* keyEvent);

    void timerEvent(QTimerEvent* timerEvent);
    void injectTimerEvent(QTimerEvent* timerEvent);

private:
    Ogre::SceneManager* sceneManager_;
    Ogre::Camera* camera_;

    Ogre::SceneNode* node_;

    QPoint mousePosition_;
    QSet<Qt::Key> keys_;

    Ogre::Real xRotation_;
    Ogre::Real yRotation_;

    float y_;

    QBasicTimer mTimer;
};

class MouseDrivenCameraController : public AbstractCameraController {
public:
    MouseDrivenCameraController(QWidget* widget, Ogre::SceneManager* sceneManager, Ogre::Camera* camera);
    ~MouseDrivenCameraController();

    Ogre::SceneManager* getSceneManager() const;
    Ogre::Camera* getCamera() const;

    float y() const;
    void setY(float y);

    float minDistance() const;
    void setMinDistance(float distance);

    float maxDistance() const;
    void setMaxDistance(float distance);

    float zoomSpeed() const;
    void setZoomSpeed(float speed);

private:
    Ogre::SceneManager* mSceneManager;
    Ogre::Camera* mCamera;

    Ogre::SceneNode* mNode;

    float mMinDistance;
    float mMaxDistance;
    float mZoomSpeed;
    float mY;

    Ogre::Real mRotationX;
    Ogre::Real mRotationY;

    QPoint mMousePosition;
    QBasicTimer mTimer;
    QSet<Qt::Key> mKeys;

    void injectMousePressEvent(QMouseEvent* mouseEvent);
    void injectMouseMoveEvent(QMouseEvent* mouseEvent);

    void injectWheelEvent(QWheelEvent* wheelEvent);

    void injectKeyPressEvent(QKeyEvent* keyEvent);
    void injectKeyReleaseEvent(QKeyEvent* keyEvent);

    void timerEvent(QTimerEvent* timerEvent);
};
