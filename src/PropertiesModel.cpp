/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "PropertiesModel.h"
#include <QtCore/qsize.h>
#include <QtGui/qcolor.h>

PropertiesModel::PropertiesModel(QObject* parent)
	: QAbstractTableModel(parent)
{
	mProperties = new PropertyList;
}

PropertiesModel::~PropertiesModel() {
	delete mProperties;
}

PropertyList PropertiesModel::properties() const {
	return *mProperties;
}

void PropertiesModel::setProperties(PropertyList properties) {
	//if (mProperties != properties) {
		beginResetModel();
		*mProperties = properties;
		endResetModel();
	//}
}

int PropertiesModel::rowCount(const QModelIndex& parent) const {
	return parent.isValid() ? 0 : mProperties->size() + 1;
}

int	PropertiesModel::columnCount(const QModelIndex& parent) const {
	return parent.isValid() ? 0 : 2;
}

Qt::ItemFlags PropertiesModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    if (index.row() < mProperties->size() || index.column() == 0)
        flags |= Qt::ItemIsEditable;

    return flags;
}

QVariant PropertiesModel::data(const QModelIndex& index, int role) const {
	if (role == Qt::DisplayRole || role == Qt::EditRole) {
		if (index.row() < mProperties->size())
			return (*mProperties)[index.row()][index.column()];

		return (index.column() == 0) && (role == Qt::DisplayRole) ? tr("<New Property>") : QString();
	}

	return QVariant();
}

bool PropertiesModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	QString text = value.toString();
	switch (index.column()) {
		case 0:
			if (text.isEmpty())
				return false;

			if (index.row() < mProperties->size()) {
				Property& prop = (*mProperties)[index.row()];
				prop = Property(text, prop.value());
				emit dataChanged(index, index);
			} else {
				beginInsertRows(QModelIndex(), mProperties->size() + 1, mProperties->size() + 1);
				mProperties->push_back(Property(text));
				endInsertRows();
				//emit dataChanged(index, this->index(index.row() + 1, index.column(), index.parent()));
			}
			return true;
		case 1:
			Property& prop = (*mProperties)[index.row()];
			prop = Property(prop.key(), text);
			emit dataChanged(index, index);
			return true;
	}
	return false;
}

QVariant PropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const {
	static QString headers[2] = {
		QString(tr("Key")),
		QString(tr("Value"))
	};

	if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole) && (section < 2))
		return headers[section];

	return QVariant();
}
