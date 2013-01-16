/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtGui/qdialog.h>

class Map;
class QSpinBox;

class NewMapDialog : public QDialog {
	Q_OBJECT

	public:
		NewMapDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);

		Map* createMap();

	private:
		QSpinBox* xSize_;
		QSpinBox* ySize_;
		QSpinBox* zSize_;
};
