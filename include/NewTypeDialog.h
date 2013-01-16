/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtGui/qdialog.h>

class Type;
class TileSet;
class PreviewBox;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;

class NewTypeDialog : public QDialog {
	Q_OBJECT

	public:
		NewTypeDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);

		Type* createType(TileSet* tileSet);

	private:
		PreviewBox* mPreview;
		QComboBox* mMesh;

		QSpinBox* mX;
		QSpinBox* mY;

	private slots:
		void changeSize();
};
