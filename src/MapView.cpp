/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "MapView.h"
#include "CameraController.h"
#include "Grid.h"
#include "FloorTool.h"
#include "WallTool.h"
#include "PaintTool.h"
#include "ObjectTool.h"
#include "MapDocument.h"
#include "Map.h"
#include "MapGeometry.h"

MapView::MapView(QWidget* parent, Qt::WindowFlags flags)
	: OgreWidget(parent, flags)
	, mMode(FLOOR_MODE)
	, mFloor(0)
	, mMap(0)
{
	setMouseTracking(true);
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum));
}

QSize MapView::sizeHint() const {
	return QSize(640, 480);
}

MapView::~MapView() {
	if (isInitialised()) {
		delete mObjectTool;
		delete mPaintTool;
		delete mWallTool;
		delete mFloorTool;

		delete mCameraController;

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		root->destroySceneManager(mSceneManager);
	}
}

Mode MapView::mode() const {
	return mMode;
}

void MapView::setMode(Mode mode) {
	if (mMode == mode)
		return;

	mCurrentTool->deactivate();

	mMode = mode;

	switch (mMode) {
		case FLOOR_MODE:
			mCurrentTool = mFloorTool;
			break;
		case WALL_MODE:
			mCurrentTool = mWallTool;
			break;
		case PAINT_MODE:
			mCurrentTool = mPaintTool;
			break;
		case OBJECT_MODE:
			mCurrentTool = mObjectTool;
			break;
	}

	mCurrentTool->activate();
	update();
}

MapDocument* MapView::mapDocument() const {
	return mMap;
}

void MapView::setMapDocument(MapDocument* mapDocument) {
	if (mMap == mapDocument)
		return;

	bool activateTool = !mMap;
	mMap = mapDocument;

	Map* map = (mMap == 0) ? 0 : mMap->getMap();
	mMapGeometry->setMap(map);
	
	mFloorTool->setMap(map);
	mWallTool->setMap(map);
	mPaintTool->setMap(map);
	mObjectTool->setMap(map);

	if (mMap == 0) {
		mCurrentTool->deactivate();
		mGrid->setVisible(false);
	} else {
		mGrid->setSize(mMap->getMap()->size().x(), mMap->getMap()->size().y());
		if (activateTool)
			mCurrentTool->activate();
	}

	update();
}

int MapView::floor() const {
	return mFloor;
}

void MapView::setFloor(int floor) {
	if (!mMap || (mMap->getMap()->size().z() <= floor) || (floor < 0))
		return;

	mFloor = floor;

	mCameraController->setY(mFloor * 2.0f);

	mFloorTool->setFloor(mFloor);
	mWallTool->setFloor(mFloor);
	mPaintTool->setFloor(mFloor);
	mObjectTool->setFloor(mFloor);

	mGrid->setElevation(2.0f * mFloor);

	update();
}

Tile* MapView::tile() const {
	return mTile;
}

void MapView::setTile(Tile* tile) {
	mTile = tile;

	mFloorTool->setTile(mTile);
	mWallTool->setTile(mTile);
	mPaintTool->setTile(mTile);
}

Type* MapView::type() const {
	return mType;
}

void MapView::setType(Type* type) {
	mType = type;

	mObjectTool->setType(mType);
}

// SLOTS

void MapView::setGrid(bool enable) {
	if (enable == mGrid->isVisible())
		return;
	
	mGrid->setVisible(enable);
	update();
}

void MapView::upFloor() {
	setFloor(floor() + 1);
}

void MapView::downFloor() {
	setFloor(floor() - 1);
}

void MapView::setMode(int mode) {
	if (mMode == mode)
		return;

	mCurrentTool->deactivate();

	mMode = static_cast<Mode>(mode);
	switch (mMode) {
		case FLOOR_MODE:
			mCurrentTool = mFloorTool;
			break;
		case WALL_MODE:
			mCurrentTool = mWallTool;
			break;
		case PAINT_MODE:
			mCurrentTool = mPaintTool;
			break;
		case OBJECT_MODE:
			mCurrentTool = mObjectTool;
			break;
	}

	mCurrentTool->activate();
	update();
}

// PROTECTED

void MapView::initialise() {
	Ogre::Root* root = Ogre::Root::getSingletonPtr();

	mSceneManager = root->createSceneManager(Ogre::ST_GENERIC);
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

	mCamera = mSceneManager->createCamera("Camera");
	mCamera->setFarClipDistance(1000.0f);
	mCamera->setAutoAspectRatio(true);
	mCamera->setPosition(Ogre::Vector3(0.0f, 0.0f, 35.0f));
	mCamera->setNearClipDistance(0.1f);
	mCamera->setFarClipDistance(1000.0f);
	mCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	mCamera->setOrthoWindowWidth(35.0f);

	mViewport = renderWindow()->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

	Ogre::Light* light = mSceneManager->createLight();
	light->setPosition(-100.0f, 100.0f, 100.0f);
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(2.0f, -3.0f, -5.0f);
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

	mMapGeometry = new MapGeometry(mSceneManager);

	mCameraController = new MouseDrivenCameraController(this, mSceneManager, mCamera);
	mGrid = new Grid(mSceneManager, mViewport);

	mFloorTool = new FloorTool(mSceneManager, mMapGeometry);
	mWallTool = new WallTool(mSceneManager, mMapGeometry);
	mPaintTool = new PaintTool(mSceneManager, mMapGeometry);
	mObjectTool = new ObjectTool(mSceneManager, mMapGeometry);
	mCurrentTool = mFloorTool;
}

void MapView::keyPressEvent(QKeyEvent* keyEvent) {
	bool modified;

	//mCameraController->injectKeyPressEvent(keyEvent);
	//modified = keyEvent->isAccepted();

	modified = mCurrentTool->keyPressEvent(keyEvent); //|| modified;

	if (modified)
		update();
}

void MapView::keyReleaseEvent(QKeyEvent* keyEvent) {
	bool modified;

	//mCameraController->injectKeyReleaseEvent(keyEvent);
	//modified = keyEvent->isAccepted();

	modified = mCurrentTool->keyReleaseEvent(keyEvent); //|| modified;

	if (modified)
		update();
}

void MapView::mouseMoveEvent(QMouseEvent* mouseEvent) {
	bool modified;

	//mCameraController->injectMouseMoveEvent(mouseEvent);
	//modified = mouseEvent->isAccepted();

	Ogre::Ray ray = mCamera->getCameraToViewportRay(mouseEvent->pos().x() / static_cast<float>(width()), mouseEvent->pos().y() / static_cast<float>(height()));

	modified = mCurrentTool->mouseMoveEvent(mouseEvent, ray); //|| modified;

	if (modified)
		update();
}

void MapView::mousePressEvent(QMouseEvent* mouseEvent) {
	bool modified;

	//mCameraController->injectMousePressEvent(mouseEvent);
	//modified = mouseEvent->isAccepted();

	Ogre::Ray ray = mCamera->getCameraToViewportRay(mouseEvent->pos().x() / static_cast<float>(width()), mouseEvent->pos().y() / static_cast<float>(height()));

	modified = mCurrentTool->mousePressEvent(mouseEvent, ray); //|| modified;

	if (modified)
		update();
}

void MapView::mouseReleaseEvent(QMouseEvent* mouseEvent) {
	Ogre::Ray ray = mCamera->getCameraToViewportRay(mouseEvent->pos().x() / static_cast<float>(width()), mouseEvent->pos().y() / static_cast<float>(height()));

	bool modified = mCurrentTool->mouseReleaseEvent(mouseEvent, ray);

	if (modified)
		update();
}

void MapView::wheelEvent(QWheelEvent* wheelEvent) {
	//mCameraController->injectWheelEvent(wheelEvent);
	//if (wheelEvent->isAccepted())
	//	update();
}