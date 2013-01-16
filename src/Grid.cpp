/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Grid.h"

const Ogre::String Grid::MATERIAL_NAME = "UniformColourTransparent";

Grid::Grid(Ogre::SceneManager* sceneManager, Ogre::Viewport* viewport)
	: sceneManager_(sceneManager)
	, viewport_(viewport)
	, sceneNode_(0)
	, lines_(0)
	, sizeX_(0)
	, sizeZ_(0)
	, colour_(Ogre::ColourValue::White)
	, visible_(true)
{
	assert(sceneManager_);
	assert(viewport_);

	sceneNode_ = sceneManager_->createSceneNode();
	sceneNode_->setPosition(-0.5f, 0.0f, -0.5f);
	lines_ = sceneManager_->createManualObject();
	sceneNode_->attachObject(lines_);

	viewport_->getTarget()->addListener(this);
}

Grid::~Grid() {
	viewport_->getTarget()->removeListener(this);

	sceneManager_->destroyManualObject(lines_);
	sceneManager_->destroySceneNode(sceneNode_);
}

Ogre::SceneManager* Grid::getSceneManager() const {
	return sceneManager_;
}

Ogre::Viewport* Grid::getViewport() const {
	return viewport_;
}

const Ogre::ColourValue& Grid::getColour() const {
	return colour_;
}

void Grid::setColour(const Ogre::ColourValue& colour) {
	colour_ = colour;
}

bool Grid::isVisible() const {
	return visible_;
}

void Grid::setVisible(bool visible) {
	visible_ = visible;
}

size_t Grid::getSizeX() const {
	return sizeX_;
}

void Grid::setSizeX(size_t size) {
	if (sizeX_ != size) {
		sizeX_ = size;
		resizeGeometry();
	}
}

size_t Grid::getSizeZ() const {
	return sizeZ_;
}

void Grid::setSizeZ(size_t size) {
	if (sizeZ_ != size) {
		sizeZ_ = size;
		resizeGeometry();
	}
}

void Grid::setSize(size_t sizeX, size_t sizeZ) {
	if ((sizeX_ != sizeX) || (sizeZ_ != sizeZ)) {
		sizeX_ = sizeX;
		sizeZ_ = sizeZ;
		resizeGeometry();
	}
}

Ogre::Real Grid::getElevation() const {
	return sceneNode_->getPosition().y;
}

void Grid::setElevation(Ogre::Real elevation) {
	Ogre::Vector3 position = sceneNode_->getPosition();
	position.y = elevation;
	sceneNode_->setPosition(position);
}

void Grid::preViewportUpdate(const Ogre::RenderTargetViewportEvent& viewportEvent) {
	if ((viewport_ == viewportEvent.source) && visible_) {
		Ogre::Camera* camera = viewport_->getCamera();
		if (camera->getProjectionType() != Ogre::PT_ORTHOGRAPHIC)
			return;

		Ogre::Real cosAngle = abs(camera->getDerivedDirection().y);
		if (Ogre::Math::RealEqual(cosAngle, 0.0f))
			return;

		Ogre::Real length = (camera->getOrthoWindowWidth() + camera->getOrthoWindowHeight()) / cosAngle;
		if (Ogre::Math::RealEqual(length, 0.0f))
			return;

		Ogre::Real pixelsPerSquare = (viewport_->getActualWidth() + viewport_->getActualHeight()) / length;

		if (pixelsPerSquare > 10.0f) {
			if (lines_->getNumSections() > 0) {
				Ogre::Real alpha = Ogre::Math::Clamp((pixelsPerSquare - 10.0f) * 0.2f, 0.0f, 1.0f);
				lines_->getSection(0)->setCustomParameter(0, Ogre::Vector4(colour_.r, colour_.g, colour_.b, colour_.a * alpha));
			}

			sceneManager_->getRootSceneNode()->addChild(sceneNode_);
		}
	}
}

void Grid::postViewportUpdate(const Ogre::RenderTargetViewportEvent& viewportEvent) {
	if (viewport_ == viewportEvent.source) {
		if (sceneNode_->isInSceneGraph()) {
			sceneManager_->getRootSceneNode()->removeChild(sceneNode_);
		}
	}
}

void Grid::resizeGeometry() {
	if (sizeX_ * sizeZ_ == 0) {
		lines_->clear();
		return;
	}

	if (lines_->getNumSections() > 0)
		lines_->beginUpdate(0);
	else
		lines_->begin(MATERIAL_NAME, Ogre::RenderOperation::OT_LINE_LIST);

	for (size_t i = 0; i <= sizeX_; ++i) {
		lines_->position(static_cast<Ogre::Real>(i), 0.0f, 0.0f);
		lines_->position(static_cast<Ogre::Real>(i), 0.0f, static_cast<Ogre::Real>(sizeZ_));
	}

	for (size_t i = 0; i <= sizeZ_; ++i) {
		lines_->position(0.0f, 0.0f, static_cast<Ogre::Real>(i));
		lines_->position(static_cast<Ogre::Real>(sizeX_), 0.0f, static_cast<Ogre::Real>(i));
	}

	lines_->end();
}

//---------------------------------------------------------------------------------------

MovableGrid::MovableGrid()
	: mX(1)
	, mY(1)
	, mColour(Ogre::ColourValue::White)
{
	init();
}

MovableGrid::MovableGrid(const Ogre::String& name)
	: Ogre::SimpleRenderable(name)
	, mX(1)
	, mY(1)
	, mColour(Ogre::ColourValue::White)
{
	init();
}

MovableGrid::~MovableGrid() {
	delete mRenderOp.vertexData;
	mRenderOp.vertexData = 0;
}

const Ogre::String& MovableGrid::getMovableType() const {
	static Ogre::String type = "MovableGrid";
	return type;
}

Ogre::Real MovableGrid::getBoundingRadius() const {
	return mBoundingRadius;
}

Ogre::Real MovableGrid::getSquaredViewDepth(const Ogre::Camera* camera) const {
	return getParentNode()->getSquaredViewDepth(camera);
}

const Ogre::ColourValue& MovableGrid::colour() const {
	return mColour;
}

void MovableGrid::setColour(const Ogre::ColourValue& colour) {
	mColour = colour;
	setCustomParameter(0, Ogre::Vector4(mColour.r, mColour.g, mColour.b, mColour.a));
}

void MovableGrid::getSize(size_t& x, size_t& y) const {
	x = mX; y = mY;
}

void MovableGrid::setSize(size_t x, size_t y) {
	mX = x; mY = y;
	update();
}

void MovableGrid::init() {
	mRenderOp.operationType = Ogre::RenderOperation::OT_LINE_LIST;
	mRenderOp.srcRenderable = this;
	mRenderOp.useIndexes = false;
	mRenderOp.indexData = 0;

	mRenderOp.vertexData = new Ogre::VertexData;
	mRenderOp.vertexData->vertexStart = 0;
	mRenderOp.vertexData->vertexCount = 0;
	mRenderOp.vertexData->vertexDeclaration->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION, 0);

	update();

	setMaterial("UniformColourTransparent");
	setColour(mColour);
}

void MovableGrid::update() {
	mRenderOp.vertexData->vertexBufferBinding->unsetAllBindings();

	Ogre::HardwareBufferManager* bufferManager = Ogre::HardwareBufferManager::getSingletonPtr();

	size_t vertexSize = Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	size_t vertexCount = (mX + mY + 2) * 2;

	Ogre::HardwareVertexBufferSharedPtr vertexBuffer = bufferManager->createVertexBuffer(vertexSize, vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	Ogre::Real* buffer = static_cast<Ogre::Real*>(vertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	Ogre::Real halfX = mX * 0.5f;
	Ogre::Real halfY = mY * 0.5f;

	for (size_t i = 0; i < (mY + 1); ++i) {
		buffer[0] = -halfX; buffer[1] = 0.0f; buffer[2] = -halfY + i;
		buffer[3] = halfX; buffer[4] = 0.0f; buffer[5] = -halfY + i;
		buffer += 6;
	}

	for (size_t i = 0; i < (mX + 1); ++i) {
		buffer[0] = -halfX + i; buffer[1] = 0.0f; buffer[2] = -halfY;
		buffer[3] = -halfX + i; buffer[4] = 0.0f; buffer[5] = halfY;

		buffer += 6;
	}

	vertexBuffer->unlock();

	mRenderOp.vertexData->vertexCount = vertexCount;
	mRenderOp.vertexData->vertexBufferBinding->setBinding(0, vertexBuffer);

	Ogre::AxisAlignedBox boundingBox(-halfX, 0.0f, -halfY, halfX, 0.0f, halfY);
	setBoundingBox(boundingBox);

	mBoundingRadius = Ogre::Math::Sqrt(halfX * halfX + halfY * halfY);
}