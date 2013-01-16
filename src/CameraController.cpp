/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "CameraController.h"
#include <QtGui/qwidget.h>

AbstractCameraController::AbstractCameraController(QWidget* widget)
	: mWidget(widget)
{
	mWidget->installEventFilter(this);
}

AbstractCameraController::~AbstractCameraController() {
	mWidget->removeEventFilter(this);
}

bool AbstractCameraController::eventFilter(QObject* watched, QEvent* e) {
	if (mWidget != watched)
		return false;

	if (e->type() == QEvent::KeyPress)
		injectKeyPressEvent(static_cast<QKeyEvent*>(e));
	else if (e->type() == QEvent::KeyRelease)
		injectKeyReleaseEvent(static_cast<QKeyEvent*>(e));
	else if (e->type() == QEvent::MouseButtonPress)
		injectMousePressEvent(static_cast<QMouseEvent*>(e));
	else if (e->type() == QEvent::MouseMove)
		injectMouseMoveEvent(static_cast<QMouseEvent*>(e));
	else if (e->type() == QEvent::Wheel)
		injectWheelEvent(static_cast<QWheelEvent*>(e));
	else
		return false;

	return false;
}

void AbstractCameraController::update() {
	mWidget->update();
}

//---------------------------------------------------------------------------------------

CameraController::CameraController(QWidget* widget, Ogre::SceneManager* sceneManager, Ogre::Camera* camera)
	: AbstractCameraController(widget)
	, sceneManager_(sceneManager)
	, camera_(camera)
	, node_(0)
	, mousePosition_(0, 0)
	, xRotation_(0.0f)
	, yRotation_(0.0f)
	, y_(0.0f)
{
	assert(sceneManager_);
	assert(camera_);

	node_ = sceneManager_->getRootSceneNode()->createChildSceneNode();
	camera->detachFromParent();
	node_->attachObject(camera);

	node_->yaw(Ogre::Radian(Ogre::Degree(xRotation_ + 45.0f)));
	node_->pitch(Ogre::Radian(Ogre::Degree(-45.0f - yRotation_)));
}

CameraController::~CameraController() {
	sceneManager_->destroySceneNode(node_);
}

Ogre::SceneManager* CameraController::getSceneManager() const {
	return sceneManager_;
}

Ogre::Camera* CameraController::getCamera() const {
	return camera_;
}

float CameraController::getY() const {
	return y_;
}

void CameraController::setY(float y) {
	Ogre::Vector3 position = node_->getPosition();
	position.y = y;
	node_->setPosition(position);
}

void CameraController::injectMousePressEvent(QMouseEvent* mouseEvent) {
	if (mouseEvent->button() == Qt::RightButton) {
		mousePosition_ = mouseEvent->pos();
		mouseEvent->accept();
	} else
		mouseEvent->ignore();
}

void CameraController::injectMouseMoveEvent(QMouseEvent* mouseEvent) {
	if ((mouseEvent->buttons() & Qt::RightButton) == Qt::RightButton) {
		QPoint difference = mouseEvent->pos() - mousePosition_;

		if (!difference.isNull()) {
			xRotation_ = fmod(xRotation_ - difference.x() * 0.36f, 360.0f);
			//yRotation_ = std::max(std::min(yRotation_ + difference.y() * 0.36f, 44.0f), -134.0f);
			yRotation_ = std::max(std::min(yRotation_ + difference.y() * 0.36f, 44.0f), 0.0f);

			node_->resetOrientation();
			node_->yaw(Ogre::Radian(Ogre::Degree(xRotation_ + 45.0f)));
			node_->pitch(Ogre::Radian(Ogre::Degree(-45.0f - yRotation_)));

			mousePosition_ = mouseEvent->pos();
			update();
		}

		mouseEvent->accept();
	} else
		mouseEvent->ignore();
}

void CameraController::injectWheelEvent(QWheelEvent* wheelEvent) {
	if (wheelEvent->orientation() == Qt::Vertical) {
		if (wheelEvent->delta() != 0) {
			Ogre::Real degrees = wheelEvent->delta() * 0.125f;

			//float cameraDistance = camera_->getPosition().z;
			//camera_->setPosition(0.0f, 0.0f, cameraDistance - (degrees * 0.2f));
			camera_->setOrthoWindowWidth(camera_->getOrthoWindowWidth() - (degrees * 0.2f));
			update();
		}
		wheelEvent->accept();
	} else
		wheelEvent->ignore();
}

void CameraController::injectKeyPressEvent(QKeyEvent* keyEvent) {
	if ((keyEvent->key() == Qt::Key_Up) || (keyEvent->key() == Qt::Key_Down) || (keyEvent->key() == Qt::Key_Left) || (keyEvent->key() == Qt::Key_Right)) {
		keys_ += static_cast<Qt::Key>(keyEvent->key());
		if (!mTimer.isActive())
			mTimer.start(0, this);
		keyEvent->accept();
	}
}

void CameraController::injectKeyReleaseEvent(QKeyEvent* keyEvent) {
	if ((keyEvent->key() == Qt::Key_Up) || (keyEvent->key() == Qt::Key_Down) || (keyEvent->key() == Qt::Key_Left) || (keyEvent->key() == Qt::Key_Right)) {
		keys_ -= static_cast<Qt::Key>(keyEvent->key());
		if (keys_.empty() && mTimer.isActive())
			mTimer.stop();
	}
}

void CameraController::timerEvent(QTimerEvent* timerEvent) {
	Ogre::Vector3 cameraPosition = node_->getPosition();
	cameraPosition.y = y_;

	Ogre::Vector3 cameraDirection = getCamera()->getDerivedDirection();
	cameraDirection.y = 0.0f;
	cameraDirection.normalise();

	Ogre::Vector3 cameraOffset(0.0f);

	if (keys_.contains(Qt::Key_Up))
		cameraOffset += cameraDirection;
	if (keys_.contains(Qt::Key_Down))
		cameraOffset -= cameraDirection;
	if (keys_.contains(Qt::Key_Left))
		cameraOffset -= cameraDirection.crossProduct(Ogre::Vector3::UNIT_Y);
	if (keys_.contains(Qt::Key_Right))
		cameraOffset += cameraDirection.crossProduct(Ogre::Vector3::UNIT_Y);

	cameraOffset.normalise();
	cameraPosition += cameraOffset * 0.1f;
	node_->setPosition(cameraPosition);

	update();
}

void CameraController::injectTimerEvent(QTimerEvent* timerEvent) {
	this->timerEvent(timerEvent);
}

//---------------------------------------------------------------------------------------

MouseDrivenCameraController::MouseDrivenCameraController(QWidget* widget, Ogre::SceneManager* sceneManager, Ogre::Camera* camera)
	: AbstractCameraController(widget)
	, mSceneManager(sceneManager)
	, mCamera(camera)
	, mNode(0)
	, mMinDistance(1.0f)
	, mMaxDistance(100.0f)
	, mZoomSpeed(1.0f)
	, mY(0.0f)
	, mRotationX(0.0f)
	, mRotationY(0.0f)
{
	mNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mCamera->detachFromParent();
	mNode->attachObject(camera);

	mNode->yaw(Ogre::Radian(Ogre::Degree(mRotationX + 45.0f)));
	mNode->pitch(Ogre::Radian(Ogre::Degree(-45.0f - mRotationY)));
}

MouseDrivenCameraController::~MouseDrivenCameraController() {
	mSceneManager->destroySceneNode(mNode);
}

Ogre::SceneManager* MouseDrivenCameraController::getSceneManager() const {
	return mSceneManager;
}

Ogre::Camera* MouseDrivenCameraController::getCamera() const {
	return mCamera;
}

Ogre::Real MouseDrivenCameraController::y() const {
	return mY;
}

void MouseDrivenCameraController::setY(Ogre::Real y) {
	mY = y;

	Ogre::Ray line(mNode->getPosition(), mNode->getOrientation().zAxis());
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, mY);

	std::pair<bool, Ogre::Real> result = line.intersects(plane);
	
	Ogre::Vector3 newPosition = line.getPoint(result.second);
	newPosition.y = mY;

	mNode->setPosition(newPosition);
}

void MouseDrivenCameraController::injectMousePressEvent(QMouseEvent* mouseEvent) {
	if (mouseEvent->button() != Qt::MiddleButton) {
		mouseEvent->ignore();
		return;
	}

	mMousePosition = mouseEvent->pos();
	mouseEvent->accept();
}

void MouseDrivenCameraController::injectMouseMoveEvent(QMouseEvent* mouseEvent) {
	if ((mouseEvent->buttons() & Qt::MiddleButton) != Qt::MiddleButton) {
		mouseEvent->ignore();
		return;
	}

	QPoint difference = mouseEvent->pos() - mMousePosition;

	if (!difference.isNull()) {
		mRotationX = fmod(mRotationX - difference.x() * 0.36f, 360.0f);
		mRotationY = Ogre::Math::Clamp(mRotationY + difference.y() * 0.36f, 0.0f, 44.0f);

		mNode->resetOrientation();
		mNode->yaw(Ogre::Radian(Ogre::Degree(mRotationX + 45.0f)));
		mNode->pitch(Ogre::Radian(Ogre::Degree(-45.0f - mRotationY)));

		mMousePosition = mouseEvent->pos();
		update();
	}

	mouseEvent->accept();
}

void MouseDrivenCameraController::injectWheelEvent(QWheelEvent* wheelEvent) {
	if (wheelEvent->orientation() != Qt::Vertical) {
		wheelEvent->ignore();
		return;
	}

	if (wheelEvent->delta() != 0) {
		Ogre::Real degrees = wheelEvent->delta() * 0.125f;
		Ogre::Real newWindowHeight = mCamera->getOrthoWindowHeight() - degrees * 0.2f;
		newWindowHeight = Ogre::Math::Clamp(newWindowHeight, 4.0f, 64.0f);

		mCamera->setOrthoWindowHeight(newWindowHeight);
		update();
	}
	
	wheelEvent->accept();
}

void MouseDrivenCameraController::injectKeyPressEvent(QKeyEvent* keyEvent) {
	if ((keyEvent->key() == Qt::Key_Up) || (keyEvent->key() == Qt::Key_Down) || (keyEvent->key() == Qt::Key_Left) || (keyEvent->key() == Qt::Key_Right)) {
		mKeys += static_cast<Qt::Key>(keyEvent->key());
		if (!mTimer.isActive())
			mTimer.start(0, this);
		keyEvent->accept();
	} else
		keyEvent->ignore();
}

void MouseDrivenCameraController::injectKeyReleaseEvent(QKeyEvent* keyEvent) {
	if ((keyEvent->key() == Qt::Key_Up) || (keyEvent->key() == Qt::Key_Down) || (keyEvent->key() == Qt::Key_Left) || (keyEvent->key() == Qt::Key_Right)) {
		mKeys -= static_cast<Qt::Key>(keyEvent->key());
		if (mKeys.empty() && mTimer.isActive())
			mTimer.stop();
		keyEvent->accept();
	} else
		keyEvent->ignore();
}

void MouseDrivenCameraController::timerEvent(QTimerEvent* timerEvent) {
	Ogre::Vector3 cameraPosition = mNode->getPosition();
	cameraPosition.y = mY;

	Ogre::Vector3 cameraDirection = getCamera()->getDerivedDirection();
	cameraDirection.y = 0.0f;
	cameraDirection.normalise();

	Ogre::Vector3 cameraOffset(0.0f);

	if (mKeys.contains(Qt::Key_Up))
		cameraOffset += cameraDirection;
	if (mKeys.contains(Qt::Key_Down))
		cameraOffset -= cameraDirection;
	if (mKeys.contains(Qt::Key_Left))
		cameraOffset -= cameraDirection.crossProduct(Ogre::Vector3::UNIT_Y);
	if (mKeys.contains(Qt::Key_Right))
		cameraOffset += cameraDirection.crossProduct(Ogre::Vector3::UNIT_Y);

	cameraOffset.normalise();
	cameraPosition += cameraOffset * 0.1f;
	mNode->setPosition(cameraPosition);

	update();
}

