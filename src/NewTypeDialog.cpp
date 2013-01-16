/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "NewTypeDialog.h"
#include "TileSet.h"
#include "Type.h"
#include "ResourceModel.h"
#include "PreviewBox.h"
#include <QtGui/qboxlayout.h>
#include <QtGui/qformlayout.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qdialogbuttonbox.h>
#include <QtCore/qsettings.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qcompleter.h>
#include <QtGui/qfilesystemmodel.h>
#include <QtGui/qlabel.h>
#include <OGRE/OgreMeshManager.h>

NewTypeDialog::NewTypeDialog(QWidget* parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	setWindowTitle(tr("New Type"));
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	QBoxLayout* layout = new QBoxLayout(QBoxLayout::Down, this);
	layout->setSizeConstraint(QLayout::SetFixedSize);

	mPreview = new PreviewBox;
	layout->addWidget(mPreview);

	QFormLayout* formLayout = new QFormLayout;

	ResourceGroupModel* model = new ResourceGroupModel(this);
	model->setPattern("*.mesh");
	model->setResourceGroup("General");
	Ogre::ResourceGroupManager* fesf = Ogre::ResourceGroupManager::getSingletonPtr();

	QBoxLayout* sizeLayout = new QBoxLayout(QBoxLayout::LeftToRight);

	QLabel* xLabel = new QLabel("X:");
	sizeLayout->addWidget(xLabel);

	mX = new QSpinBox;
	mX->setMinimum(1);
	connect(mX, SIGNAL(valueChanged(int)), this, SLOT(changeSize()));
	sizeLayout->addWidget(mX, 1);

	QLabel* yLabel = new QLabel("Y:");
	sizeLayout->addWidget(yLabel);

	mY = new QSpinBox;
	mY->setMinimum(1);
	connect(mY, SIGNAL(valueChanged(int)), this, SLOT(changeSize()));
	sizeLayout->addWidget(mY, 1);

	layout->addLayout(sizeLayout);

	mMesh = new QComboBox;
	mMesh->setModel(model);
	connect(mMesh, SIGNAL(currentIndexChanged(const QString&)), mPreview, SLOT(setMesh(QString)));
	formLayout->addRow(tr("Mesh:"), mMesh);

	layout->addLayout(formLayout);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	layout->addWidget(buttonBox);

	//QSettings settings;
	//settings.beginGroup("NewMapDialog");
	//xSize_->setValue(settings.value("xSize", 32).toInt());
	//ySize_->setValue(settings.value("ySize", 32).toInt());
	//zSize_->setValue(settings.value("zSize", 1).toInt());
	//settings.endGroup();
}

Type* NewTypeDialog::createType(TileSet* tileSet) {
	Type* type = 0;
	if (exec() == QDialog::Accepted) {
		//QSettings settings;
		//settings.beginGroup("NewMapDialog");
		//settings.setValue("xSize", xSize_->value());
		//settings.setValue("ySize", ySize_->value());
		//settings.setValue("zSize", zSize_->value());
		//settings.endGroup();

		type = new Type;
		type->setSize(QSize(mX->value(), mY->value()));
		type->setMesh(mPreview->mesh());
		type->setOffset(mPreview->offset());
		type->setRotation(mPreview->rotation());
		type->setScale(mPreview->scale());
	}
	return type;
}

void NewTypeDialog::changeSize() {
	QSize size(mX->value(), mY->value());
	mPreview->setSize(size);
}