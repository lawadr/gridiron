/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "TileSet.h"
#include "Tile.h"
#include "Type.h"
#include "Category.h"

/*ItemSetModel::ItemSetModel(QObject* parent) {
}

ItemSetModel::~ItemSetModel() {
}

Qt::ItemFlags ItemSetModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	flags |= Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    return flags;
}

QModelIndex ItemSetModel::index(int row, int column, const QModelIndex& parent) const {
	Category* category = mRoot;
	if (parent.isValid())
		category = static_cast<Category*>(parent.internalPointer());

	return (row < category->childCount()) ? createIndex(row, column, category->child(row)) : QModelIndex();
}

QModelIndex ItemSetModel::parent(const QModelIndex& index) const {
	Item* item = static_cast<Item*>(index.internalPointer());
	Category* parent = item->parent();
	return (mRoot != parent) ? createIndex(parent->childNumber(), 0, parent) : QModelIndex();
}

int ItemSetModel::rowCount(const QModelIndex& parent) const {
	Item* item = mRoot;
	if (parent.isValid())
		item = static_cast<Item*>(parent.internalPointer());

	return item->isCategory() ? item->toCategory()->childCount() : 0;
}

int ItemSetModel::columnCount(const QModelIndex& parent) const {
	return 1;
}

QVariant ItemSetModel::data(const QModelIndex& index, int role) const {
	if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
		Item* item = static_cast<Item*>(index.internalPointer());
		return item->name();
	}
	return QVariant();
}

bool ItemSetModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	if (role == Qt::EditRole) {
		Item* item = static_cast<Item*>(index.internalPointer());
		item->setName(value.toString());
		emit dataChanged(index, index);
		return true;
	}
	return false;
}*/

//---------------------------------------------------------------------------------------

ItemSet::ItemSet()
	: mRoot(0)
	, mCurrent(0)
{
	mRoot = new Category;
}

ItemSet::~ItemSet() {
	delete mRoot;

	for (ItemList::iterator it = mItems.begin(); it != mItems.end(); ++it) {
		delete *it;
	}
}

Item* ItemSet::current() const {
	return mCurrent;
}

void ItemSet::setCurrent(Item* item) {
	if (contains(item))
		mCurrent = item;
}

void ItemSet::destroyItem(Item* item) {
	mItems.removeOne(item);
	delete item;
}

bool ItemSet::contains(Item* item) const {
	return mItems.contains(item);
}

Category* ItemSet::root() const {
	return mRoot;
}

//---------------------------------------------------------------------------------------

TileSet::TileSet() {
	root()->setName("Tiles");
}

TileSet::~TileSet() {
}

Item* TileSet::createItem() {
	Tile* tile = new Tile(nextId());
	mItems.push_back(tile);
	mTiles.push_back(tile);
	root()->addChild(tile);
	return tile;
}

void TileSet::destroyItem(Item* item) {
	mTiles.removeOne(static_cast<Tile*>(item));
	ItemSet::destroyItem(item);
}

Tile* TileSet::createTile() {
	return static_cast<Tile*>(createItem());
}

QList<Tile*> TileSet::tiles() const {
	return mTiles;
}

int TileSet::nextId() const {
	return (mItems.empty()) ? 1 : static_cast<Tile*>(mItems.last())->id() + 1;
}

//---------------------------------------------------------------------------------------

TypeSet::TypeSet() {
	root()->setName("Types");
}

TypeSet::~TypeSet() {
}

Item* TypeSet::createItem() {
	Type* type = new Type;
	mItems.push_back(type);
	root()->addChild(type);
	return type;
}
