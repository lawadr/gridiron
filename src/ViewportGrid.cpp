/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "ViewportGrid.h"

const Ogre::String ViewportGrid::MATERIAL_NAME = "UniformColourTransparent";

ViewportGrid::ViewportGrid(Ogre::SceneManager* sceneManager, Ogre::Viewport* viewport)
	: sceneManager_(sceneManager)
	, viewport_(viewport)
	, orientationNode_(0)
	, translationNode_(0)
	, lines_(0)
	, size_(0)
	, plane_(Ogre::Vector3::UNIT_Y, 0.0f)
	, colour_(Ogre::ColourValue::White)
	, visible_(true)
{
	assert(sceneManager_);
	assert(viewport_);

	orientationNode_ = sceneManager_->createSceneNode();
	translationNode_ = orientationNode_->createChildSceneNode();

	lines_ = sceneManager_->createManualObject();
	lines_->setDynamic(true);
	translationNode_->attachObject(lines_);

	viewport_->getTarget()->addListener(this);
}

ViewportGrid::~ViewportGrid() {
	viewport_->getTarget()->removeListener(this);

	sceneManager_->destroyManualObject(lines_);
	sceneManager_->destroySceneNode(translationNode_);
	sceneManager_->destroySceneNode(orientationNode_);
}

Ogre::SceneManager* ViewportGrid::getSceneManager() const {
	return sceneManager_;
}

Ogre::Viewport* ViewportGrid::getViewport() const {
	return viewport_;
}

const Ogre::ColourValue& ViewportGrid::getColour() const {
	return colour_;
}

void ViewportGrid::setColour(const Ogre::ColourValue& colour) {
	colour_ = colour;
}

const Ogre::Vector3& ViewportGrid::getOrigin() const {
	return orientationNode_->getPosition();
}

void ViewportGrid::setOrigin(const Ogre::Vector3& origin) {
	orientationNode_->setPosition(origin);
	plane_.redefine(orientationNode_->getOrientation().yAxis(), orientationNode_->getPosition());
}

const Ogre::Quaternion& ViewportGrid::getOrientation() const {
	return orientationNode_->getOrientation();
}

void ViewportGrid::setOrientation(const Ogre::Quaternion& orientation) {
	orientationNode_->setOrientation(orientation);
	plane_.redefine(orientationNode_->getOrientation().yAxis(), orientationNode_->getPosition());
}

bool ViewportGrid::getVisible() const {
	return visible_;
}

void ViewportGrid::setVisible(bool visible) {
	visible_ = visible;
}

void ViewportGrid::preViewportUpdate(const Ogre::RenderTargetViewportEvent& viewportEvent) {
	if ((viewport_ == viewportEvent.source) && visible_) {
		Ogre::Camera* camera = viewport_->getCamera();
		if (camera->getProjectionType() != Ogre::PT_ORTHOGRAPHIC)
			return;

		Ogre::Real cosAngle = abs(orientationNode_->getOrientation().yAxis().dotProduct(-camera->getDerivedDirection()));
		if (Ogre::Math::RealEqual(cosAngle, 0.0f))
			return;

		Ogre::Real length = (camera->getOrthoWindowWidth() + camera->getOrthoWindowHeight()) / cosAngle;
		if (Ogre::Math::RealEqual(length, 0.0f))
			return;

		Ogre::Real pixelsPerSquare = (viewport_->getActualWidth() + viewport_->getActualHeight()) / length;

		if (pixelsPerSquare > 10.0f) {
			Ogre::Ray ray =	camera->getCameraToViewportRay(0.5f, 0.5f);
			std::pair<bool, Ogre::Real> result = ray.intersects(plane_);

			if (result.first) {
				Ogre::Vector3 point = orientationNode_->convertWorldToLocalPosition(ray.getPoint(result.second));
				translationNode_->setPosition(floor(point.x + 0.5f), 0.0f, floor(point.z + 0.5f));

				size_t requiredSize = static_cast<size_t>(Ogre::Math::Ceil(length + 0.5f) + 1.5f);
				requiredSize += requiredSize % 2;

				if (size_ < requiredSize) {
					size_ = requiredSize;
					resizeGeometry();
				}

				if (lines_->getNumSections() > 0) {
					Ogre::Real alpha = Ogre::Math::Clamp((pixelsPerSquare - 10.0f) * 0.2f, 0.0f, 1.0f);
					lines_->getSection(0)->setCustomParameter(0, Ogre::Vector4(colour_.r, colour_.g, colour_.b, colour_.a * alpha));
				}

				sceneManager_->getRootSceneNode()->addChild(orientationNode_);
			}
		}
	}
}

void ViewportGrid::postViewportUpdate(const Ogre::RenderTargetViewportEvent& viewportEvent) {
	if (viewport_ == viewportEvent.source) {
		if (orientationNode_->isInSceneGraph()) {
			sceneManager_->getRootSceneNode()->removeChild(orientationNode_);
		}
	}
}

void ViewportGrid::resizeGeometry() {
	Ogre::Real size = static_cast<Ogre::Real>(size_);
	Ogre::Real offset = size * 0.5f;

	if (lines_->getNumSections() > 0)
		lines_->beginUpdate(0);
	else
		lines_->begin(MATERIAL_NAME, Ogre::RenderOperation::OT_LINE_LIST);

	for (size_t i = 0; i <= size_; ++i) {
		lines_->position(-offset, 0.0f, static_cast<Ogre::Real>(i) - offset);
		lines_->position(offset, 0.0f, static_cast<Ogre::Real>(i) - offset);

		lines_->position(static_cast<Ogre::Real>(i) - offset, 0.0f, -offset);
		lines_->position(static_cast<Ogre::Real>(i) - offset, 0.0f, offset);
	}

	lines_->end();
}