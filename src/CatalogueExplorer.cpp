/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "CatalogueExplorer.h"
#include "Catalogue.h"
#include "TileSet.h"
#include "Item.h"
#include "CategoryModel.h"
#include "Tile.h"
#include "Type.h"
#include "NewTileDialog.h"
#include "NewTypeDialog.h"
#include <QtGui/qtoolbox.h>
#include <QtGui/qtreeview.h>
#include <QtGui/qmenu.h>
#include <QtGui/qboxlayout.h>

CatalogueExplorer::CatalogueExplorer(QWidget* parent, Qt::WindowFlags flags)
	: QWidget(parent, flags)
	, mCatalogue(0)
{
	mToolBox = new QToolBox(this);

	QString titles[3] = {tr("Tiles"), tr("Walls"), tr("Objects")};
	QIcon icon = QIcon::fromTheme("document-save-all");

	for (int i = 0; i < 3; ++i) {
		mItemSets[i].view = new QTreeView(this);
		mItemSets[i].view->setContextMenuPolicy(Qt::CustomContextMenu);
		mItemSets[i].view->setHeaderHidden(true);
		mItemSets[i].view->setEditTriggers(QAbstractItemView::SelectedClicked);
		mItemSets[i].view->setDragEnabled(true);
		mItemSets[i].view->setDefaultDropAction(Qt::MoveAction);
		mItemSets[i].view->setDropIndicatorShown(true);
		mItemSets[i].view->setSelectionMode(QAbstractItemView::ExtendedSelection);
		mItemSets[i].view->viewport()->setAcceptDrops(true);
		
		connect(mItemSets[i].view, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
		connect(mItemSets[i].view, SIGNAL(activated(const QModelIndex&)), this, SLOT(itemActivated(const QModelIndex&)));

		mItemSets[i].model = 0;

		mToolBox->addItem(mItemSets[i].view, icon, titles[i]);
	}

	mCreateTileAction = new QAction(tr("New Tile"), this);
	mCreateTypeAction = new QAction(tr("New Type"), this);
	mCreateCategoryAction = new QAction(tr("New Category"), this);
	mDestroyItemAction = new QAction(tr("Delete"), this);

	connect(mCreateTileAction, SIGNAL(triggered()), this, SLOT(createTile()));
	connect(mCreateTypeAction, SIGNAL(triggered()), this, SLOT(createType()));
	connect(mCreateCategoryAction, SIGNAL(triggered()), this, SLOT(createCategory()));
	connect(mDestroyItemAction, SIGNAL(triggered()), this, SLOT(destroyItem()));
	
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(mToolBox);
}

CatalogueExplorer::~CatalogueExplorer() {
}

Catalogue* CatalogueExplorer::catalogue() const {
	return mCatalogue;
}

void CatalogueExplorer::setCatalogue(Catalogue* catalogue) {
	if (mCatalogue == catalogue)
		return;
	clear();

	mCatalogue = catalogue;
		
	mItemSets[0].model = new CategoryModel(this);
	mItemSets[0].model->setItemSet(mCatalogue->tileSet());
	mItemSets[0].view->setModel(mItemSets[0].model);

	mItemSets[2].model = new CategoryModel(this);
	mItemSets[2].model->setItemSet(mCatalogue->typeSet());
	mItemSets[2].view->setModel(mItemSets[2].model);

	for (int i = 0; i < 3; ++i) {
		connect(mItemSets[i].view->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(itemSelected(const QModelIndex&)));
	}
}

void CatalogueExplorer::clear() {
	mCatalogue = 0;

	if (mItemSets[0].model) delete mItemSets[0].model;
	if (mItemSets[2].model) delete mItemSets[2].model;
	mItemSets[0].model = 0;
	mItemSets[2].model = 0;
}

void CatalogueExplorer::createTile() {
	int index = mToolBox->currentIndex();
	QModelIndex modelIndex = mItemSets[index].view->currentIndex();

	NewTileDialog* dialog = new NewTileDialog(this);
	Item* item = dialog->createTile(mCatalogue->tileSet());

	if (item) {
		modelIndex = mItemSets[index].model->addItem(item, modelIndex);
		mItemSets[index].view->setCurrentIndex(modelIndex);
		mItemSets[index].view->edit(modelIndex);
	}

	delete dialog;
}

void CatalogueExplorer::createType() {
	int index = mToolBox->currentIndex();
	QModelIndex modelIndex = mItemSets[index].view->currentIndex();

	NewTypeDialog* dialog = new NewTypeDialog(this);
	Item* item = dialog->createType(mCatalogue->tileSet());

	if (item) {
		modelIndex = mItemSets[index].model->addItem(item, modelIndex);
		mItemSets[index].view->setCurrentIndex(modelIndex);
		mItemSets[index].view->edit(modelIndex);
	}

	delete dialog;
}

void CatalogueExplorer::createCategory() {
	int index = mToolBox->currentIndex();
	QModelIndex modelIndex = mItemSets[index].view->currentIndex();
	modelIndex = mItemSets[index].model->createCategory(modelIndex);
	mItemSets[index].view->setCurrentIndex(modelIndex);
	mItemSets[index].view->edit(modelIndex);
}

void CatalogueExplorer::destroyItem() {
	int index = mToolBox->currentIndex();
	QModelIndex modelIndex = mItemSets[index].view->currentIndex();
	mItemSets[index].model->destroyChild(modelIndex.row(), modelIndex.parent());
}

void CatalogueExplorer::showContextMenu(const QPoint& position) {
	int index = mToolBox->currentIndex();
	if (!mItemSets[index].model) return;

	QModelIndex modelIndex = mItemSets[index].view->currentIndex();
	if (Item* item = mItemSets[index].model->item(modelIndex)) {
		QMenu* menu = new QMenu(this);

		if (item->isCategory()) {
			QMenu* addMenu = new QMenu(tr("&Add"), menu);
			addMenu->addAction(mCreateTileAction);
			addMenu->addAction(mCreateTypeAction);
			addMenu->addAction(mCreateCategoryAction);
			menu->addMenu(addMenu);
		}
		menu->addAction(mDestroyItemAction);

		menu->exec(mItemSets[index].view->viewport()->mapToGlobal(position));
		delete menu;
	}
}

void CatalogueExplorer::itemSelected(const QModelIndex& current) {
	int index = mToolBox->currentIndex();
	Item* item = mItemSets[index].model->item(current);
	emit itemSelected(item);
}

void CatalogueExplorer::itemActivated(const QModelIndex& index) {
	Item* item = mItemSets[mToolBox->currentIndex()].model->item(index);
	if (!item->isCategory()) {
		switch (mToolBox->currentIndex()) {
			case 0:
				emit tileActivated(static_cast<Tile*>(item));
				break;
			case 2:
				emit typeActivated(static_cast<Type*>(item));
				break;
		}
	}
}
