/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtGui/qevent.h>

class SceneEvent : public QEvent {
public:
    SceneEvent(QEvent::Type type)
        : QEvent(type)
    {
    }

    virtual ~SceneEvent() {
    }

private:
};

#include <OGRE/OgreRay.h>

class SceneMouseEvent : public SceneEvent {
public:
    static const QEvent::Type MOVE_TYPE;
    static const QEvent::Type PRESS_TYPE;
    static const QEvent::Type RELEASE_TYPE;
    static const QEvent::Type DOUBLE_CLICK_TYPE;

    SceneMouseEvent(QEvent::Type type)
        : SceneEvent(type)
    {
    }

    ~SceneMouseEvent() {
    }

    Qt::MouseButton button() const {
        return mButton;
    }

    void setButton(Qt::MouseButton button) {
        mButton = button;
    }

    Qt::MouseButtons buttons() const {
        return mButtons;
    }

    void setButtons(const Qt::MouseButtons& buttons) {
        mButtons = buttons;
    }

    const Ogre::Ray& ray() const {
        return mRay;
    }

    void setRay(const Ogre::Ray& ray) {
        mRay = ray;
    }

private:
    Qt::MouseButton mButton;
    Qt::MouseButtons mButtons;
    Ogre::Ray mRay;
};
