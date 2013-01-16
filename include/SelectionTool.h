/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Tool.h"
#include <QtCore/qset.h>

class Object;

class SelectionTool : public Tool {
	public:
		SelectionTool(Ogre::SceneManager* sceneManager, MapGeometry* mapGeometry);

		void activate();
		void deactivate();
		
		bool mousePressEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);
		bool mouseMoveEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);
		bool mouseReleaseEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);

	private:
		MapGeometry* mMapGeometry;
		QSet<Object*> mSelection;

		bool mMoving;
};
