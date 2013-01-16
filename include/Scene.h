/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtCore/qobject.h>

class SceneView;
class SceneItem;
class SceneMouseEvent;

class QKeyEvent;

namespace Ogre {
	class SceneManager;
}

class Scene : public QObject {
	Q_OBJECT

	public:
		friend class SceneView;

		Scene(QObject* parent = 0);
		~Scene();

		void addItem(SceneItem* item);
		void removeItem(SceneItem* item);

	signals:
		void changed();

	protected:
		Ogre::SceneManager* sceneManager() const;

		virtual void keyPressEvent(QKeyEvent* keyEvent);
		virtual void keyReleaseEvent(QKeyEvent* keyEvent);

		virtual void mouseMoveEvent(SceneMouseEvent* mouseEvent);
		virtual void mousePressEvent(SceneMouseEvent* mouseEvent);
		virtual void mouseReleaseEvent(SceneMouseEvent* mouseEvent);

		void update();

	private:
		Ogre::SceneManager* mSceneManager;
		QList<SceneItem*> mSceneItems;
};
