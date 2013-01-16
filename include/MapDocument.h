/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtCore/qobject.h>

class Map;
class QUndoStack;

class MapDocument : public QObject {
	Q_OBJECT

	public:
		MapDocument();
		MapDocument(Map* map);
		~MapDocument();

		bool isModified() const;

		Map* getMap() const;

		QUndoStack* getUndoStack() const;

		QString getFileName() const;
		void setFileName(QString fileName);

		bool save();

	private:
		Map* map_;
		QUndoStack* undoStack_;

		bool isModified_;
		QString fileName_;
};
