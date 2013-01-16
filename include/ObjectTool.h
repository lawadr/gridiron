/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Tool.h"
#include "Angle.h"

class Type;

class ObjectTool : public Tool {
	public:
		ObjectTool(Ogre::SceneManager* sceneManager, MapGeometry* mapGeometry);
		~ObjectTool();

		Type* type() const;
		void setType(Type* type);

		void activate();
		void deactivate();
		
		bool mousePressEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);
		bool mouseMoveEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);
		bool mouseReleaseEvent(QMouseEvent* mouseEvent, const Ogre::Ray& ray);

	private:
		Type* mType;

		MapGeometry* mMapGeometry;

		Ogre::SceneNode* mNode;
		Ogre::SceneNode* mNode2;
		Ogre::Entity* mObject;

		QPoint mPosition;
		Angle mAngle;

		void realign() const;

		bool collisionCheck(const QRect& bounds) const;

		bool isOutsideMap(const QRect& bounds) const;
		bool floorCollisionCheck(const QRect& bounds) const;
		bool wallCollisionCheck(const QRect& bounds) const;
		bool objectCollisionCheck(const QRect& bounds) const;
};

