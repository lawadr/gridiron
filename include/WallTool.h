/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Tool.h"

class Tile;

class WallTool : public Tool {
	public:
		WallTool(Ogre::SceneManager* sceneManager, MapGeometry* mapGeometry);
		~WallTool();

		Tile* tile() const;
		void setTile(Tile* tile);

		bool mousePressEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);
		bool mouseMoveEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);
		bool mouseReleaseEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);

	private:
		bool mSelecting;
		QPoint mStart;
		QPoint mCurrent;

		Ogre::SceneNode* mNode;
		Ogre::ManualObject* mWall;

		MapGeometry* mMapGeometry;
		Tile* mTile;

		QPoint calculatePosition(const Ogre::Ray& ray);
};
