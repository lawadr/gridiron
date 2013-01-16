/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "OgreWidget.h"

class MouseDrivenCameraController;
class Grid;

class Tool;
class FloorTool;
class WallTool;
class PaintTool;
class ObjectTool;

class MapDocument;
class MapGeometry;
class Tile;
class Type;

enum Mode {
	FLOOR_MODE = 0,
	WALL_MODE = 1,
	PAINT_MODE = 2,
	OBJECT_MODE = 3,
	SELECTION_MODE = 4,
	DEFAULT_MODE = 0
};

class MapView : public OgreWidget {
	Q_OBJECT

	public:
		MapView(QWidget* parent = 0, Qt::WindowFlags flags = 0);
		~MapView();

		virtual QSize sizeHint() const;

		Mode mode() const;
		void setMode(Mode mode);

		MapDocument* mapDocument() const;
		void setMapDocument(MapDocument* mapDocument);

		int floor() const;
		void setFloor(int floor);

		Tile* tile() const;
		Type* type() const;

	public slots:
		void setGrid(bool enable);
		void upFloor();
		void downFloor();
		void setMode(int mode);

		void setTile(Tile* tile);
		void setType(Type* type);

	signals:
		void hoverChanged(QPoint position);

	protected:
		virtual void initialise();

		virtual void keyPressEvent(QKeyEvent* keyEvent);
		virtual void keyReleaseEvent(QKeyEvent* keyEvent);

		virtual void mouseMoveEvent(QMouseEvent* mouseEvent);
		virtual void mousePressEvent(QMouseEvent* mouseEvent);
		virtual void mouseReleaseEvent(QMouseEvent* mouseEvent);

		virtual void wheelEvent(QWheelEvent* wheelEvent);

	private:
		Ogre::SceneManager* mSceneManager;
		Ogre::Viewport* mViewport;
		Ogre::Camera* mCamera;

		MouseDrivenCameraController* mCameraController;
		Grid* mGrid;

		Mode mMode;
		Tool* mCurrentTool;
		FloorTool* mFloorTool;
		WallTool* mWallTool;
		PaintTool* mPaintTool;
		ObjectTool* mObjectTool;

		MapDocument* mMap;
		int mFloor;
		MapGeometry* mMapGeometry;
		Tile* mTile;
		Type* mType;
};
