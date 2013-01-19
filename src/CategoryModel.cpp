/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "CategoryModel.h"
#include "TileSet.h"
#include "Category.h"
#include <QtGui/qicon.h>
#include <QtCore/qset.h>
#include <QtCore/qmimedata.h>

CategoryModel::CategoryModel(QObject* parent)
    : QAbstractItemModel(parent)
    , mItemSet(0)
    , mRoot(0)
{
}

CategoryModel::~CategoryModel() {
}

ItemSet* CategoryModel::itemSet() const {
    return mItemSet;
}

void CategoryModel::setItemSet(ItemSet* itemSet) {
    if (mItemSet != itemSet) {
        beginResetModel();
        mItemSet = itemSet;
        mRoot = itemSet->root();
        endResetModel();
    }
}

Item* CategoryModel::item(const QModelIndex& index) const {
    if (!index.isValid())
        return 0;
    return static_cast<Item*>(index.internalPointer());
}

QModelIndex CategoryModel::addItem(Item* item, const QModelIndex& parent) {
    Category* category = mRoot;
    if (parent.isValid()) {
        Item* item = static_cast<Item*>(parent.internalPointer());
        if (!item->isCategory())
            return QModelIndex();
        category = item->toCategory();
    }

    QSet<QString> names;
    for (int n = 0; n < category->childCount(); ++n) {
        names.insert(category->child(n)->name().toLower());
    }

    if (names.contains(item->name().toLower())) {
        int n = 1;
        QString name = item->name().toLower();
        name.append("%1");
        while (names.contains(name.arg(QString::number(n)))) {
            ++n;
        }
        item->setName(item->name() + QString::number(n));
    }

    beginInsertRows(parent, category->childCount(), category->childCount());
    category->addChild(item);
    endInsertRows();

    return createIndex(category->childCount() - 1, 0, item);
}

Item* CategoryModel::removeItem(int row, const QModelIndex& parent) {
    Category* category = mRoot;
    if (parent.isValid())
        category = static_cast<Category*>(parent.internalPointer());

    beginRemoveRows(parent, row, row);
    Item* item = category->removeChild(row);
    endRemoveRows();

    return item;
}

QModelIndex CategoryModel::createCategory(const QModelIndex& parent) {
    Category* category = new Category;
    category->setName("NewCategory");
    QModelIndex index = addItem(category, parent);
    if (!index.isValid())
        delete category;

    return index;
}

void CategoryModel::destroyCategory(int row, const QModelIndex& parent) {
    Category* category = mRoot;
    if (parent.isValid())
        category = static_cast<Category*>(parent.internalPointer());

    if (category->child(row)->isCategory()) {
        Item* item = removeItem(row, parent);
        delete item;
    }
}

QModelIndex CategoryModel::createChild(const QModelIndex& parent) {
    Category* category = mRoot;
    if (parent.isValid())
        category = static_cast<Category*>(parent.internalPointer());

    QSet<QString> names;
    for (int n = 0; n < category->childCount(); ++n) {
        names.insert(category->child(n)->name().toLower());
    }

    int n = 1;
    QString name("newcategory%1");
    while (names.contains(name.arg(QString::number(n)))) {
        ++n;
    }

    name = "NewCategory";
    name.append(QString::number(n));


    beginInsertRows(parent, category->childCount(), category->childCount());

    Category* child = new Category;
    category->addChild(child);
    child->setName(name);

    endInsertRows();

    return createIndex(category->childCount() - 1, 0, child);
}

void CategoryModel::destroyChild(int row, const QModelIndex& parent) {
    Category* category = mRoot;
    if (parent.isValid())
        category = static_cast<Category*>(parent.internalPointer());

    beginRemoveRows(parent, row, row);
    delete category->removeChild(row);
    endRemoveRows();
}

Qt::ItemFlags CategoryModel::flags(const QModelIndex& index) const {
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.isValid()) {
        Item* item = static_cast<Item*>(index.internalPointer());
        if (item->isCategory())
            flags |= Qt::ItemIsDropEnabled;
        if (mRoot != item)
            flags |= Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
    }

    return flags;
}

QModelIndex CategoryModel::index(int row, int column, const QModelIndex& parent) const {
    /*Category* category = mRoot;
    if (parent.isValid())
    category = static_cast<Category*>(parent.internalPointer());

    if (row < category->childCount())
    return createIndex(row, column, category->child(row));
    else
    return QModelIndex();*/

    if (!parent.isValid())
        return (row == 0) ? createIndex(row, column, mRoot) : QModelIndex();

    Category* category = static_cast<Category*>(parent.internalPointer());
    return (row < category->childCount()) ? createIndex(row, column, category->child(row)) : QModelIndex();
}

QModelIndex CategoryModel::parent(const QModelIndex& index) const {
    /*Item* item = static_cast<Item*>(index.internalPointer());
    Category* parent = item->parent();
    if (mRoot == parent)
    return QModelIndex();

    return createIndex(parent->childNumber(), 0, parent);*/

    Item* item = static_cast<Item*>(index.internalPointer());
    Category* parent = item->parent();
    return (mRoot != item) ? createIndex(parent->childNumber(), 0, parent) : QModelIndex();
}

int CategoryModel::rowCount(const QModelIndex& parent) const {
    /*if (!parent.isValid())
    return mRoot->childCount();

    Item* item = static_cast<Item*>(parent.internalPointer());
    return (item->isCategory()) ? static_cast<Category*>(item)->childCount() : 0;*/

    if (!parent.isValid())
        return 1;

    Item* item = static_cast<Item*>(parent.internalPointer());
    return (item->isCategory()) ? static_cast<Category*>(item)->childCount() : 0;
}

int CategoryModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QVariant CategoryModel::data(const QModelIndex& index, int role) const {
    Item* item = static_cast<Item*>(index.internalPointer());
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return item->name();
    } else if (role == Qt::DecorationRole) {
        QIcon icon;
        if (item->isCategory())
            icon = QIcon::fromTheme("document-save-all");
        else
            icon = QIcon::fromTheme("edit-undo");
        return icon;
    }
    return QVariant();
}

bool CategoryModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        Item* item = static_cast<Item*>(index.internalPointer());
        item->setName(value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool CategoryModel::insertRows(int row, int count, const QModelIndex& parent) {
    return createChild(parent).isValid();
}

bool CategoryModel::removeRows(int row, int count, const QModelIndex& parent) {
    return false;
}

QStringList CategoryModel::mimeTypes() const {
    QStringList mimeTypes;
    mimeTypes << "application/editor.categorymodel.index";
    return mimeTypes;
}

Qt::DropActions	CategoryModel::supportedDropActions() const {
    return Qt::CopyAction | Qt::MoveAction;
}

QMimeData* CategoryModel::mimeData(const QModelIndexList& indexes) const {
    QMimeData *mimeData = new QMimeData;
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    for (QModelIndexList::const_iterator it = indexes.begin(); it != indexes.end(); ++it) {
        quint64 address = reinterpret_cast<quint64>(it->internalPointer());
        stream << address;
    }

    mimeData->setData("application/editor.categorymodel.index", encodedData);
    return mimeData;
}

bool CategoryModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) {
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("application/editor.categorymodel.index"))
        return false;

    QByteArray encodedData = data->data("application/editor.categorymodel.index");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    QList<Item*> items;

    while (!stream.atEnd()) {
        quint64 address;
        stream >> address;
        Item* item = reinterpret_cast<Item*>(address);
        items.push_back(item);
    }

    Category* category = static_cast<Category*>(parent.internalPointer());

    for (QList<Item*>::const_iterator it = items.begin(); it != items.end(); ++it) {
        Item* item = *it;
        if (!item->isCategory() || !item->toCategory()->isAncestorOf(category)) {
            QModelIndex parentIndex = createIndex(item->parent()->childNumber(), 1, item->parent());
            if (parentIndex != parent) {
                beginMoveRows(parentIndex, item->childNumber(), item->childNumber(), parent, rowCount(parent));
                category->addChild(item);
                endMoveRows();
            }
        }
    }

    return true;
}
