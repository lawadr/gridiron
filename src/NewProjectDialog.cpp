/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "NewProjectDialog.h"
#include <QtGui/qformlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qfiledialog.h>

NewProjectDialog::NewProjectDialog(QWidget* parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	QBoxLayout* layout = new QBoxLayout(QBoxLayout::Down, this);

	QFormLayout* formLayout = new QFormLayout(this);

	nameEdit_ = new QLineEdit(this);
	formLayout->addRow(tr("Name:"), nameEdit_);
	connect(nameEdit_, SIGNAL(textChanged(const QString&)), this, SLOT(locationChange()));
	
	QBoxLayout* locationLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	
	locationEdit_ = new QLineEdit(this);
	locationLayout->addWidget(locationEdit_);
	connect(locationEdit_, SIGNAL(textChanged(const QString&)), this, SLOT(locationChange()));

	QPushButton* browseButton = new QPushButton("Browse", this);
	locationLayout->addWidget(browseButton);
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	
	formLayout->addRow(tr("Location:"), locationLayout);

	layout->addLayout(formLayout);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	okButton_ = buttonBox->button(QDialogButtonBox::Ok);
	okButton_->setEnabled(false);

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	layout->addWidget(buttonBox);

	setLayout(layout);
	setWindowTitle("New Project");
}

QString NewProjectDialog::getName() const {
	return nameEdit_->text();
}

QString NewProjectDialog::getLocation() const {
	return locationEdit_->text();
}

void NewProjectDialog::nameChange() {
	okButton_->setEnabled(!nameEdit_->text().isEmpty());
}

void NewProjectDialog::locationChange() {
	if (!nameEdit_->text().isEmpty() && !locationEdit_->text().isEmpty()) {
		QDir directory(locationEdit_->text());
		okButton_->setEnabled(directory.exists() && directory.isAbsolute());
	} else
		okButton_->setEnabled(false);
}

void NewProjectDialog::browse() {
	QString project = QFileDialog::getExistingDirectory(this, tr("Project Location"), "");
	if (!project.isEmpty())
		locationEdit_->setText(project);
}
