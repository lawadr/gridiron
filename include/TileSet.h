/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtCore/qlist.h>
#include <QtCore/qabstractitemmodel.h>

class Item;
class Tile;
class Category;

/*class ItemSetModel : public QAbstractItemModel {
	Q_OBJECT

	public:
		ItemSetModel(QObject* parent = 0);
		~ItemSetModel();

		Qt::ItemFlags flags(const QModelIndex& index) const;

		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex& index) const;

		int rowCount(const QModelIndex& parent = QModelIndex()) const;
		int columnCount(const QModelIndex& parent = QModelIndex()) const;

		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	protected:
		typedef QList<Item*> ItemList;
		ItemList mItems;	

	private:
		Category* mRoot;
		Item* mCurrent;
};*/

class ItemSet {
	public:
		ItemSet();
		virtual ~ItemSet();

		Item* current() const;
		void setCurrent(Item* item);

		virtual Item* createItem() = 0;
		virtual void destroyItem(Item* item);

		bool contains(Item* item) const;

		Category* root() const;

	protected:
		typedef QList<Item*> ItemList;
		ItemList mItems;

	private:
		Category* mRoot;
		Item* mCurrent;
};

class TileSet : public ItemSet {
	public:
		TileSet();
		~TileSet();

		Item* createItem();
		void destroyItem(Item* item);

		QList<Tile*> tiles() const;

		Tile* createTile();

	private:
		QList<Tile*> mTiles;

		int nextId() const;
};

class TypeSet : public ItemSet {
	public:
		TypeSet();
		~TypeSet();

		Item* createItem();

	private:
};
