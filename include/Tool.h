/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#ifndef TOOL_H
#define TOOL_H

#include <OGRE/Ogre.h>
#include <QtCore/qpoint.h>
#include <QtGui/qevent.h>

class Map;
class MapGeometry;

namespace Ogre {
	class Ray;
}

class Tool {
	public:
		Tool(Ogre::SceneManager* sceneManager)
			: mSceneManager(sceneManager)
			, mMap(0)
			, mFloor(0)
		{
		}

		virtual ~Tool() {
		}

		void setMap(Map* map) {
			mMap = map;
		}

		void setFloor(int floor) {
			mFloor = floor;
		}

		virtual void activate() {
		}

		virtual void deactivate() {
		}

		virtual bool keyPressEvent(QKeyEvent* keyEvent) {
			return false;
		}

		virtual bool keyReleaseEvent(QKeyEvent* keyEvent) {
			return false;
		}

		virtual bool mousePressEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
			return false;
		}

		virtual bool mouseMoveEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
			return false;
		}

		virtual bool mouseReleaseEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray) {
			return false;
		}

	protected:
		Ogre::SceneManager* mSceneManager;
		Map* mMap;
		int mFloor;
};

#endif