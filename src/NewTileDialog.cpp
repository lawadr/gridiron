/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "NewTileDialog.h"
#include "TileSet.h"
#include "Tile.h"
#include "ResourceModel.h"
#include <QtGui/qboxlayout.h>
#include <QtGui/qformlayout.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qdialogbuttonbox.h>
#include <QtCore/qsettings.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qcompleter.h>
#include <QtGui/qfilesystemmodel.h>
#include <OGRE/OgreMaterialManager.h>

NewTileDialog::NewTileDialog(QWidget* parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
{
    setWindowTitle(tr("New Tile"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QBoxLayout* layout = new QBoxLayout(QBoxLayout::Down, this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    QFormLayout* formLayout = new QFormLayout;

    ResourceModel* model = new ResourceModel(this);
    model->setResourceManager(Ogre::MaterialManager::getSingletonPtr());
    model->setResourceGroup("General");

    mName = new QLineEdit;
    formLayout->addRow(tr("Name: "), mName);

    mMaterial = new QComboBox;
    mMaterial->setModel(model);
    formLayout->addRow(tr("Material: "), mMaterial);

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

Tile* NewTileDialog::createTile(TileSet* tileSet) {
    Tile* tile = 0;
    if (exec() == QDialog::Accepted) {
        //QSettings settings;
        //settings.beginGroup("NewMapDialog");
        //settings.setValue("xSize", xSize_->value());
        //settings.setValue("ySize", ySize_->value());
        //settings.setValue("zSize", zSize_->value());
        //settings.endGroup();

        tile = static_cast<Tile*>(tileSet->createItem());
        tile->setName(mName->text());
        tile->setMaterial(mMaterial->currentText().toStdString());
    }
    return tile;
}
