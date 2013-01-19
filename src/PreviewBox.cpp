/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "PreviewBox.h"
#include "Grid.h"
#include <QtGui/qevent.h>

PreviewBox::PreviewBox(QWidget* parent, Qt::WindowFlags flags)
    : OgreWidget(parent, flags)
    , mSceneManager(0)
    , mCamera(0)
    , mViewport(0)
    , mNode(0)
    , mEntity(0)
    , mGrid(0)
    , mOffset(Ogre::Vector3::ZERO)
    , mRotation(0.0f)
    , mScale(1.0f)
    , mSize(1, 1)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
}

PreviewBox::~PreviewBox() {
    if (isInitialised()) {
        mSceneManager->extractMovableObject(mGrid);
        delete mGrid;
        Ogre::Root::getSingletonPtr()->destroySceneManager(mSceneManager);
    }
}

QSize PreviewBox::sizeHint() const {
    return QSize(400, 400);
}

const Ogre::String& PreviewBox::mesh() const {
    return mMesh;
}

void PreviewBox::setMesh(const Ogre::String& mesh) {
    if (mMesh == mesh)
        return;

    mMesh = mesh;

    if (mSceneManager) {
        if (mEntity) {
            mSceneManager->destroyEntity(mEntity);
            mEntity = 0;
        }
        if (!mMesh.empty()) {
            mEntity = mSceneManager->createEntity(mMesh);
            mNode->attachObject(mEntity);
        }
        update();
    }
}

void PreviewBox::setMesh(QString mesh) {
    setMesh(mesh.toStdString());
}

const Ogre::Vector3& PreviewBox::offset() const {
    return mOffset;
}

void PreviewBox::setOffset(const Ogre::Vector3& offset) {
    mOffset = offset;
    if (mNode) {
        mNode->setPosition(mOffset);
        update();
    }
}

Ogre::Radian PreviewBox::rotation() const {
    return mRotation;
}

void PreviewBox::setRotation(Ogre::Radian rotation) {
    mRotation = rotation;
    if (mNode) {
        Ogre::Quaternion orientation(mRotation, Ogre::Vector3::UNIT_Y);
        mNode->setOrientation(orientation);
        update();
    }
}

Ogre::Real PreviewBox::scale() const {
    return mScale;
}

void PreviewBox::setScale(Ogre::Real scale) {
    mScale = scale;
    if (mNode) {
        mNode->setScale(mScale, mScale, mScale);
        update();
    }
}

QSize PreviewBox::size() const {
    return mSize;
}

void PreviewBox::setSize(QSize size) {
    mSize = size;

    if (mSceneManager) {
        mGrid->setSize(size.width(), size.height());
        adjustCamera();
        update();
    }
}

void PreviewBox::mousePressEvent(QMouseEvent* mouseEvent) {
    Ogre::Viewport* viewport = viewportAt(mouseEvent->pos());
    if (viewport) {
        QSizeF size = viewportSize(viewport);
        QPointF point = viewportCoordinates(viewport, mouseEvent->pos());

        Ogre::Real x = point.x() / size.width();
        Ogre::Real y = point.y() / size.height();

        if (mouseEvent->button() == Qt::LeftButton) {
            move(viewport, x, y);
        } else if ((mouseEvent->button() == Qt::RightButton) && (mViewport == viewport)) {
            rotate(viewport, x, y);
        } else if (mouseEvent->button() == Qt::MiddleButton) {
            mMousePosition = mouseEvent->posF();
        }
    }
}

void PreviewBox::mouseMoveEvent(QMouseEvent* mouseEvent) {
    Ogre::Viewport* viewport = viewportAt(mouseEvent->pos());
    if (viewport) {
        QSizeF size = viewportSize(viewport);
        QPointF point = viewportCoordinates(viewport, mouseEvent->pos());

        Ogre::Real x = point.x() / size.width();
        Ogre::Real y = point.y() / size.height();

        if (mouseEvent->buttons().testFlag(Qt::LeftButton)) {
            move(viewport, x, y);
        } else if (mouseEvent->buttons().testFlag(Qt::RightButton) && (mViewport == viewport)) {
            rotate(viewport, x, y);			
        } else if (mouseEvent->buttons().testFlag(Qt::MiddleButton)) {
            QPointF difference = mouseEvent->posF() - mMousePosition;
            mScale += mScale * 0.01f * difference.x();
            mNode->setScale(mScale, mScale, mScale);
            mMousePosition = mouseEvent->posF();
            update();
        }
    }
}

void PreviewBox::initialise() {
    mSceneManager = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC);

    mCamera = mSceneManager->createCamera("Camera");
    mCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    mCamera->setNearClipDistance(0.1f);
    mCamera->setFarClipDistance(100.0f);
    mCamera->setOrthoWindowHeight(3.0f);
    mCamera->setAutoAspectRatio(true);

    Ogre::SceneNode* cameraNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
    cameraNode->setPosition(0.0f, 10.0f, 0.0f);
    cameraNode->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(-90.0f)), Ogre::Vector3::UNIT_X));
    cameraNode->attachObject(mCamera);

    mViewport = renderWindow()->addViewport(mCamera, 0, 0.0f, 0.0f, 0.5f, 0.5f);
    mViewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

    mCamera2 = mSceneManager->createCamera("Camera2");
    mCamera2->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    mCamera2->setNearClipDistance(0.1f);
    mCamera2->setFarClipDistance(100.0f);
    mCamera2->setOrthoWindowHeight(3.0f);
    mCamera2->setAutoAspectRatio(true);

    Ogre::SceneNode* cameraNode2 = mSceneManager->getRootSceneNode()->createChildSceneNode();
    cameraNode2->setPosition(-10.0f, 1.0f, 0.0f);
    cameraNode2->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(-90.0f)), Ogre::Vector3::UNIT_Y));
    cameraNode2->attachObject(mCamera2);

    mViewport2 = renderWindow()->addViewport(mCamera2, 1, 0.5f, 0.5f, 0.5f, 0.5f);
    mViewport2->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

    mCamera3 = mSceneManager->createCamera("Camera3");
    mCamera3->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    mCamera3->setNearClipDistance(0.1f);
    mCamera3->setFarClipDistance(100.0f);
    mCamera3->setOrthoWindowHeight(3.0f);
    mCamera3->setAutoAspectRatio(true);

    Ogre::SceneNode* cameraNode3 = mSceneManager->getRootSceneNode()->createChildSceneNode();
    cameraNode3->setPosition(0.0f, 1.0f, 10.0f);
    cameraNode3->attachObject(mCamera3);

    mViewport3 = renderWindow()->addViewport(mCamera3, 2, 0.5f, 0.0f, 0.5f, 0.5f);
    mViewport3->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

    adjustCamera();

    mNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
    mNode->setPosition(mOffset);
    mNode->setOrientation(Ogre::Quaternion(mRotation, Ogre::Vector3::UNIT_Y));
    mNode->setScale(mScale, mScale, mScale);

    if (!mMesh.empty())
        mEntity = mSceneManager->createEntity(mMesh);

    mGrid = new MovableGrid("Grid");
    mSceneManager->injectMovableObject(mGrid);
    mSceneManager->getRootSceneNode()->attachObject(mGrid);
    mGrid->setSize(mSize.width(), mSize.height());
}

void PreviewBox::adjustCamera() {
    Ogre::Real x = mSize.width() + 1.0f;
    Ogre::Real y = (mSize.height() + 1.0f) * mCamera->getAspectRatio();
    mCamera->setOrthoWindowWidth(std::max(x, y));

    Ogre::Real x2 = mSize.height() + 1.0f;
    Ogre::Real y2 = 3.0f * mCamera2->getAspectRatio();
    mCamera2->setOrthoWindowWidth(std::max(x2, y2));

    Ogre::Real x3 = mSize.width() + 1.0f;
    Ogre::Real y3 = 3.0f * mCamera2->getAspectRatio();
    mCamera3->setOrthoWindowWidth(std::max(x3, y3));

    /*if (mEntity) {
    Ogre::AxisAlignedBox box = mEntity->getWorldBoundingBox();
    box.merge(Ogre::AxisAlignedBox(0.0f, -10.0f, 0.0f, 0.0f, 10.0f, 0.0f));

    const Ogre::Vector3& min = box.getMinimum();
    const Ogre::Vector3& max = box.getMaximum();

    Ogre::SceneNode* cameraNode = mCamera->getParentSceneNode();
    cameraNode->setPosition(0.0f, max.y + mCamera->getNearClipDistance() * 2, 0.0f);
    mCamera->setFarClipDistance(box.getSize().y);
    }*/
}

void PreviewBox::move(Ogre::Viewport* viewport, Ogre::Real x, Ogre::Real y) {
    Ogre::Ray ray = viewport->getCamera()->getCameraToViewportRay(x, y);
    Ogre::Plane plane(-ray.getDirection(), mOffset);
    std::pair<bool, Ogre::Real> result = ray.intersects(plane);

    if (result.first) {
        mOffset = ray.getPoint(result.second);
        mNode->setPosition(mOffset);
        update();
    }
}

void PreviewBox::rotate(Ogre::Viewport* viewport, Ogre::Real x, Ogre::Real y) {
    Ogre::Ray ray = mCamera->getCameraToViewportRay(x, y);
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0.0f);
    std::pair<bool, Ogre::Real> result = ray.intersects(plane);

    if (result.first) {
        Ogre::Vector3 direction = ray.getPoint(result.second) - mOffset;
        direction.y = 0.0f;
        direction.normalise();
        Ogre::Quaternion orientation(Ogre::Vector3::UNIT_Y.crossProduct(direction), Ogre::Vector3::UNIT_Y, direction);
        mRotation = orientation.getYaw();
        mNode->setOrientation(orientation);
        update();
    }
}

Ogre::Viewport* PreviewBox::viewportAt(QPoint position) const {
    for (int i = 0; i < renderWindow()->getNumViewports(); ++i) {
        Ogre::Viewport* viewport = renderWindow()->getViewport(i);

        int left, top, width, height;
        viewport->getActualDimensions(left, top, width, height);
        int right = left + width;
        int bottom = top + height;

        if ((position.x() >= left) && (position.x() < right) && (position.y() >= top) && (position.y() < bottom)) {
            return viewport;
        }
    }
    return 0;
}

QPointF PreviewBox::viewportCoordinates(Ogre::Viewport* viewport, QPoint position) const {
    int left, top, width, height;
    viewport->getActualDimensions(left, top, width, height);

    return QPointF(position.x() - left, position.y() - top);
}

QSizeF PreviewBox::viewportSize(Ogre::Viewport* viewport) const {
    int left, top, width, height;
    viewport->getActualDimensions(left, top, width, height);

    return QSizeF(width, height);
}