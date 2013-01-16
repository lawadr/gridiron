/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Tool.h"

class Tile;

class PaintTool : public Tool {
	public:
		PaintTool(Ogre::SceneManager* sceneManager, MapGeometry* mapGeometry);
		~PaintTool();

		Tile* tile() const;
		void setTile(Tile* tile);

		bool mouseMoveEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);
		bool mouseReleaseEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);

	private:
		int mSide;
		QPoint mPosition;

		Ogre::SceneNode* mNode;
		Ogre::ManualObject* mRectangle;

		MapGeometry* mMapGeometry;
		Tile* mTile;

		void createRectangle();
};
