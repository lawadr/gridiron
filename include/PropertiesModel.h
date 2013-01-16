/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtCore/qabstractitemmodel.h>
#include "Property.h"

class PropertiesModel : public QAbstractTableModel {
	Q_OBJECT

	public:
		PropertiesModel(QObject* parent = 0);
		~PropertiesModel();

		PropertyList properties() const;
		void setProperties(PropertyList properties);

		int rowCount(const QModelIndex& parent = QModelIndex()) const;
		int	columnCount(const QModelIndex& parent = QModelIndex()) const;

		Qt::ItemFlags flags(const QModelIndex& index) const;

		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		
	private:
		PropertyList* mProperties;
		QList<QString> keys_;
};
