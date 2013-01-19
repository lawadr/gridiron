/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "OgreWidget.h"
#include <QtCore/qmap.h>

class Grid;
class MovableGrid;

/*class ViewportWidget;

class Viewport : public QObject {
Q_OBJECT

public:
Viewport(ViewportWidget* parent)
: QObject(parent)
{
}
~Viewport();

Ogre::Viewport* viewport() const {
return mViewport;
}

private:
ViewportWidget* mParent;
Ogre::Viewport* mViewport;
Ogre::Camera* mCamera;

};

class ViewportWidget : public OgreWidget {
public:
ViewportWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0)
: OgreWidget(parent, flags)
{
}

void addViewport(Viewport* viewport) {
if (!mViewports.contains(viewport)) {
mViewports.push_back(viewport);
}
}

void removeViewport(Viewport* viewport) {
mViewports.removeAll(viewport);
if (mActiveViewport == viewport) {
mActiveViewport = 0;
}
}

protected:
virtual void keyPressEvent(QKeyEvent* keyEvent) {
if (mActiveViewport) {
mActiveViewport->event(keyEvent);
}
}

virtual void keyReleaseEvent(QKeyEvent* keyEvent) {
if (mActiveViewport) {
mActiveViewport->event(keyEvent);
}
}

virtual void mousePressEvent(QMouseEvent* mouseEvent) {
mActiveViewport = viewportAt(mouseEvent->pos());
if (mActiveViewport) {
mActiveViewport->event(mouseEvent);
}
}

virtual void mouseMoveEvent(QMouseEvent* mouseEvent) {
if (mActiveViewport) {
mActiveViewport->event(mouseEvent);
}
}

virtual void mouseReleaseEvent(QMouseEvent* mouseEvent) {
if (mActiveViewport) {
mActiveViewport->event(mouseEvent);
}
}

virtual void wheelEvent(QWheelEvent* wheelEvent) {
if (mActiveViewport) {
mActiveViewport->event(wheelEvent);
}
}

Viewport* viewportAt(QPoint position) const {
for (ViewportList::const_iterator it = mViewports.begin(); it != mViewports.end(); ++it) {
Ogre::Viewport* viewport = (*it)->viewport();

int left, top, width, height;
viewport->getActualDimensions(left, top, width, height);
int right = left + width;
int bottom = top + height;

if ((position.x() >= left) && (position.x() < right) && (position.y() >= top) && (position.y() < bottom)) {
return *it;
}
}
return 0;
}

private:
typedef QList<Viewport*> ViewportList;
QList<Viewport*> mViewports;
Viewport* mActiveViewport;
};*/

class PreviewBox : public OgreWidget {
    Q_OBJECT

public:
    PreviewBox(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~PreviewBox();

    virtual QSize sizeHint() const;

    const Ogre::String& mesh() const;
    void setMesh(const Ogre::String& mesh);

    const Ogre::Vector3& offset() const;
    void setOffset(const Ogre::Vector3& offset);

    Ogre::Radian rotation() const;
    void setRotation(Ogre::Radian rotation);

    Ogre::Real scale() const;
    void setScale(Ogre::Real scale);

    QSize size() const;
    void setSize(QSize size);

    public slots:
        void setMesh(QString mesh);

protected:
    virtual void mousePressEvent(QMouseEvent* mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent* mouseEvent);

    virtual void initialise();

private:
    Ogre::String mMesh;

    Ogre::Vector3 mOffset;
    Ogre::Radian mRotation;
    Ogre::Real mScale;

    QSize mSize;

    Ogre::SceneManager* mSceneManager;

    Ogre::Camera* mCamera;
    Ogre::Camera* mCamera2;
    Ogre::Camera* mCamera3;

    Ogre::Viewport* mViewport;
    Ogre::Viewport* mViewport2;
    Ogre::Viewport* mViewport3;

    MovableGrid* mGrid;
    Ogre::SceneNode* mNode;
    Ogre::Entity* mEntity;

    QPointF mMousePosition;

    void adjustCamera();
    void move(Ogre::Viewport* viewport, Ogre::Real x, Ogre::Real y);
    void rotate(Ogre::Viewport* viewport, Ogre::Real x, Ogre::Real y);

    Ogre::Viewport* viewportAt(QPoint position) const;
    QPointF viewportCoordinates(Ogre::Viewport* viewport, QPoint position) const;
    QSizeF viewportSize(Ogre::Viewport* viewport) const;
};
