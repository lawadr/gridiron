/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "NewMapDialog.h"
#include "Map.h"
#include <QtGui/qboxlayout.h>
#include <QtGui/qformlayout.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qdialogbuttonbox.h>
#include <QtCore/qsettings.h>

NewMapDialog::NewMapDialog(QWidget* parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
{
    setWindowTitle(tr("New Map"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QBoxLayout* layout = new QBoxLayout(QBoxLayout::Down, this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    QFormLayout* formLayout = new QFormLayout;

    xSize_ = new QSpinBox;
    xSize_->setRange(1, 1024);
    xSize_->setAccelerated(true);
    formLayout->addRow(tr("X: "), xSize_);

    ySize_ = new QSpinBox;
    ySize_->setRange(1, 1024);
    ySize_->setAccelerated(true);
    formLayout->addRow(tr("Y: "), ySize_);

    zSize_ = new QSpinBox;
    zSize_->setRange(1, 16);
    zSize_->setAccelerated(true);
    formLayout->addRow(tr("Z: "), zSize_);

    layout->addLayout(formLayout);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    layout->addWidget(buttonBox);

    QSettings settings;
    settings.beginGroup("NewMapDialog");
    xSize_->setValue(settings.value("xSize", 32).toInt());
    ySize_->setValue(settings.value("ySize", 32).toInt());
    zSize_->setValue(settings.value("zSize", 1).toInt());
    settings.endGroup();
}

Map* NewMapDialog::createMap() {
    if (exec() == QDialog::Accepted) {
        QSettings settings;
        settings.beginGroup("NewzDialog");
        settings.setValue("xSize", xSize_->value());
        settings.setValue("ySize", ySize_->value());
        settings.setValue("zSize", zSize_->value());
        settings.endGroup();

        return new Map(Size(xSize_->value(), ySize_->value(), zSize_->value()));
    } else
        return 0;
}
