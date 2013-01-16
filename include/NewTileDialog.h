/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtGui/qdialog.h>

class Tile;
class TileSet;
class QLineEdit;
class QComboBox;

class NewTileDialog : public QDialog {
	Q_OBJECT

	public:
		NewTileDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);

		Tile* createTile(TileSet* tileSet);

	private:
		QLineEdit* mName;
		QComboBox* mMaterial;
};
