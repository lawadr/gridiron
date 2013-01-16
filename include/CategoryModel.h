/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtCore/qabstractitemmodel.h>

class ItemSet;
class Category;
class Item;

class CategoryModel : public QAbstractItemModel {
	Q_OBJECT

	public:
		CategoryModel(QObject* parent = 0);
		~CategoryModel();

		ItemSet* itemSet() const;
		void setItemSet(ItemSet* itemSet);

		Item* item(const QModelIndex& index) const;

		QModelIndex addItem(Item* item, const QModelIndex& parent = QModelIndex());
		Item* removeItem(int row, const QModelIndex& parent = QModelIndex());

		QModelIndex createCategory(const QModelIndex& parent = QModelIndex());
		void destroyCategory(int row, const QModelIndex& parent = QModelIndex());

		QModelIndex createChild(const QModelIndex& parent = QModelIndex());
		void destroyChild(int row, const QModelIndex& parent = QModelIndex());

		Qt::ItemFlags flags(const QModelIndex& index) const;

		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex& index) const;

		int rowCount(const QModelIndex& parent = QModelIndex()) const;
		int columnCount(const QModelIndex& parent = QModelIndex()) const;

		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

		bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
		bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

		QStringList mimeTypes() const;
		Qt::DropActions	supportedDropActions() const;

		QMimeData* mimeData(const QModelIndexList& indexes) const;
		bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);

	private:
		ItemSet* mItemSet;
		Category* mRoot;
};
