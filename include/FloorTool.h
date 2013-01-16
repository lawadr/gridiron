/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Tool.h"

class Tile;

class FloorTool : public Tool {
	public:
		FloorTool(Ogre::SceneManager* sceneManager, MapGeometry* mapGeometry);
		~FloorTool();

		Tile* tile() const;
		void setTile(Tile* tile);

		void activate();
		void deactivate();

		bool mousePressEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);
		bool mouseMoveEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);
		bool mouseReleaseEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);

	private:
		bool mClearing;
		bool mSelecting;
		QPoint mStart;
		QPoint mCurrent;
		
		Ogre::MaterialPtr mMaterial;
		Ogre::SceneNode* mNode;
		Ogre::ManualObject* mRectangle;

		MapGeometry* mMapGeometry;
		Tile* mTile;

		QPoint calculatePosition(const Ogre::Ray& ray);
		void createRectangle();
};
