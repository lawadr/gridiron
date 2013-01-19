/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "EditorWindow.h"
#include "Map.h"
#include "TileSet.h"
#include "Tile.h"
#include "NewProjectDialog.h"
#include "NewMapDialog.h"
#include "PropertiesModel.h"
#include "MapReader.h"
#include "NewTileDialog.h"
#include "NewTypeDialog.h"
#include "CategoryModel.h"
#include "CatalogueExplorer.h"
#include <QtGui/qshortcut.h>
#include <QtGui/qevent.h>
#include <QtGui/qtabbar.h>
#include <QtGui/qtoolbox.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtoolbar.h>
#include <QtGui/qstatusbar.h>
#include <QtGui/qfilesystemmodel.h>
#include <QtGui/qstyle.h>
#include <QtCore/qsignalmapper.h>
#include <QtGui/qundoview.h>
#include <QtGui/qundogroup.h>
#include <QtGui/qgraphicsscene.h>
#include <QtGui/qgraphicsview.h>
#include <QtGui/qactiongroup.h>
#include <QtGui/qtableview.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qsortfilterproxymodel.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qdir.h>
#include <QtXml/qdom.h>
#include <QtCore/qmimedata.h>

EditorWindow::EditorWindow()
    : currentMap_(0)
    , currentItem(0)
{
    setWindowTitle(tr("Editor"));

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    undoGroup_ = new QUndoGroup(this);

    tabBar_ = new QTabBar(centralWidget);
    tabBar_->setExpanding(false);
    tabBar_->setMovable(true);
    tabBar_->setTabsClosable(true);
    tabBar_->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);

    mMapView = new MapView(centralWidget);

    centralLayout->addWidget(tabBar_);
    centralLayout->addWidget(mMapView);

    setCentralWidget(centralWidget);

    connect(tabBar_, SIGNAL(currentChanged(int)), this, SLOT(changeTab(int)));
    connect(tabBar_, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(tabBar_, SIGNAL(tabMoved(int, int)), this, SLOT(moveTab(int, int)));

    connect(mMapView, SIGNAL(hoverChanged(QPoint)), this, SLOT(updateStatus(QPoint)));

    createActions();
    createMenus();
    createToolBars();
    createDocks();
    statusBar();
}

void EditorWindow::createActions() {
    newAction_ = new QAction(QIcon::fromTheme("document-new"), tr("&New"), this);
    newAction_->setShortcut(QKeySequence::New);
    newAction_->setStatusTip(tr("Create a new map."));
    connect(newAction_, SIGNAL(triggered()), this, SLOT(newMap()));

    openAction_ = new QAction(QIcon::fromTheme("document-open"), tr("&Open"), this);
    openAction_->setShortcut(QKeySequence::Open);
    openAction_->setStatusTip(tr("Open an existing map."));
    connect(openAction_, SIGNAL(triggered()), this, SLOT(open()));

    saveAction_ = new QAction(QIcon::fromTheme("document-save"), tr("&Save"), this);
    saveAction_->setShortcut(QKeySequence::Save);
    saveAction_->setStatusTip(tr("Save the current map."));
    connect(saveAction_, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction_ = new QAction(QIcon::fromTheme("document-save-as"), tr("Save &As"), this);
    saveAsAction_->setShortcut(QKeySequence::SaveAs);
    saveAsAction_->setStatusTip(tr("Save the current map to a new file."));
    connect(saveAsAction_, SIGNAL(triggered()), this, SLOT(saveAs()));

    saveAllAction_ = new QAction(QIcon::fromTheme("document-save-all"), tr("Save A&ll"), this);
    saveAllAction_->setStatusTip(tr("Save all maps."));
    connect(saveAllAction_, SIGNAL(triggered()), this, SLOT(saveAll()));

    exitAction_ = new QAction(QIcon::fromTheme("application-exit"), tr("E&xit"), this);
    exitAction_->setShortcut(QKeySequence::Quit);
    exitAction_->setStatusTip(tr("Exit the application."));
    connect(exitAction_, SIGNAL(triggered()), this, SLOT(close()));

    undoAction_ = new QAction(QIcon::fromTheme("edit-undo"), tr("&Undo"), this);
    redoAction_ = new QAction(QIcon::fromTheme("edit-redo"), tr("&Redo"), this);

    cutAction_ = new QAction(QIcon::fromTheme("edit-cut"), tr("Cu&t"), this);
    cutAction_->setShortcut(QKeySequence::Cut);

    copyAction_ = new QAction(QIcon::fromTheme("edit-copy"), tr("&Copy"), this);
    copyAction_->setShortcut(QKeySequence::Copy);

    pasteAction_ = new QAction(QIcon::fromTheme("edit-paste"), tr("&Paste"), this);
    pasteAction_->setShortcut(QKeySequence::Paste);

    deleteAction_ = new QAction(QIcon::fromTheme("edit-delete"), tr("&Delete"), mMapView);
    deleteAction_->setShortcut(QKeySequence::Delete);
    deleteAction_->setShortcutContext(Qt::WidgetShortcut);

    gridAction_ = new QAction(QIcon::fromTheme("view-grid"), tr("&Grid"), this);
    gridAction_->setStatusTip(tr("Toggle the grid."));
    gridAction_->setCheckable(true);
    gridAction_->setChecked(true);
    connect(gridAction_, SIGNAL(toggled(bool)), mMapView, SLOT(setGrid(bool)));

    upAction_ = new QAction(QIcon::fromTheme("go-up"), tr("&Up"), this);
    upAction_->setStatusTip(tr("Increase the current level."));
    connect(upAction_, SIGNAL(triggered()), mMapView, SLOT(upFloor()));

    downAction_ = new QAction(QIcon::fromTheme("go-down"), tr("&Down"), this);
    downAction_->setStatusTip(tr("Decrease the current level."));
    connect(downAction_, SIGNAL(triggered()), mMapView, SLOT(downFloor()));

    QActionGroup* group = new QActionGroup(this);
    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), mMapView, SLOT(setMode(int)));
    //connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(changeTab(int)));

    floorAction_ = new QAction(tr("&Floor"), group);
    floorAction_->setCheckable(true);
    floorAction_->setChecked(true);
    signalMapper->setMapping(floorAction_, FLOOR_MODE);
    connect(floorAction_, SIGNAL(triggered()), signalMapper, SLOT(map()));

    wallAction_ = new QAction(tr("&Wall"), group);
    wallAction_->setCheckable(true);
    signalMapper->setMapping(wallAction_, WALL_MODE);
    connect(wallAction_, SIGNAL(triggered()), signalMapper, SLOT(map()));

    paintAction_ = new QAction(tr("&Paint"), group);
    paintAction_->setCheckable(true);
    signalMapper->setMapping(paintAction_, PAINT_MODE);
    connect(paintAction_, SIGNAL(triggered()), signalMapper, SLOT(map()));

    objectAction_ = new QAction(tr("&Object"), group);
    objectAction_->setCheckable(true);
    signalMapper->setMapping(objectAction_, OBJECT_MODE);
    connect(objectAction_, SIGNAL(triggered()), signalMapper, SLOT(map()));

    aboutAction_ = new QAction(QIcon::fromTheme("help-about"), tr("&About"), this);
    connect(aboutAction_, SIGNAL(triggered()), this, SLOT(about()));

    mAddTileAction = new QAction(tr("&Tile"), this);
    connect(mAddTileAction, SIGNAL(triggered()), this, SLOT(newTile()));

    mAddTypeAction = new QAction(tr("T&ype"), this);
    connect(mAddTypeAction, SIGNAL(triggered()), this, SLOT(newType()));


    mAddCategoryAction = new QAction(tr("&Category"), this);
    connect(mAddCategoryAction, SIGNAL(triggered()), this, SLOT(newCategory()));
}

void EditorWindow::createMenus() {
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction_);
    fileMenu->addAction(openAction_);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction_);
    fileMenu->addAction(saveAsAction_);
    fileMenu->addAction(saveAllAction_);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction_);

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction_);
    editMenu->addAction(redoAction_);
    editMenu->addSeparator();
    editMenu->addAction(cutAction_);
    editMenu->addAction(copyAction_);
    editMenu->addAction(pasteAction_);
    editMenu->addAction(deleteAction_);

    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(gridAction_);
    viewMenu->addAction(upAction_);
    viewMenu->addAction(downAction_);

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction_);
}

void EditorWindow::createToolBars() {
    QToolBar* fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAction_);
    fileToolBar->addAction(openAction_);
    fileToolBar->addAction(saveAction_);

    QToolBar* editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAction_);
    editToolBar->addAction(redoAction_);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAction_);
    editToolBar->addAction(copyAction_);
    editToolBar->addAction(pasteAction_);

    QToolBar* viewToolBar = addToolBar(tr("View"));
    viewToolBar->addAction(gridAction_);
    viewToolBar->addAction(upAction_);
    viewToolBar->addAction(downAction_);

    QToolBar* toolBar = addToolBar(tr(""));
    toolBar->addAction(floorAction_);
    toolBar->addAction(wallAction_);
    toolBar->addAction(paintAction_);
    toolBar->addAction(objectAction_);
}

void EditorWindow::createDocks() {
    QDockWidget* catalogueExplorerDock = new QDockWidget("Catalogue Explorer", this);
    mCatalogueExplorer = new CatalogueExplorer(catalogueExplorerDock);
    catalogueExplorerDock->setWidget(mCatalogueExplorer);
    addDockWidget(Qt::LeftDockWidgetArea, catalogueExplorerDock);

    connect(mCatalogueExplorer, SIGNAL(tileActivated(Tile*)), mMapView, SLOT(setTile(Tile*)));
    connect(mCatalogueExplorer, SIGNAL(typeActivated(Type*)), mMapView, SLOT(setType(Type*)));
    connect(mCatalogueExplorer, SIGNAL(itemSelected(Item*)), this, SLOT(itemSelected(Item*)));

    /*QShortcut* deleteShortcut = new QShortcut(QKeySequence::Delete, mTileView);
    deleteShortcut->setContext(Qt::WidgetShortcut);
    connect(deleteShortcut, SIGNAL(activated()), this, SLOT(deleteCategory()));
    connect(mTileView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(itemSelected(const QModelIndex&, const QModelIndex&)));*/

    mPropertiesModel = new PropertiesModel(this);

    QDockWidget* propertiesDock = new QDockWidget("Properties", this);
    mPropertiesView = new QTableView(propertiesDock);
    propertiesDock->setWidget(mPropertiesView);
    addDockWidget(Qt::RightDockWidgetArea, propertiesDock);
    mPropertiesView->setModel(mPropertiesModel);
    //properties3->setShowGrid(false);
    mPropertiesView->setTabKeyNavigation(true);
    mPropertiesView->setWordWrap(false);
    mPropertiesView->setCornerButtonEnabled(false);
    mPropertiesView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    mPropertiesView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mPropertiesView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mPropertiesView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    mPropertiesView->verticalHeader()->setHighlightSections(false);
    mPropertiesView->verticalHeader()->setClickable(false);
    mPropertiesView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    mPropertiesView->verticalHeader()->setDefaultSectionSize(mPropertiesView->verticalHeader()->minimumSectionSize());
    mPropertiesView->verticalHeader()->hide();

    mPropertiesView->horizontalHeader()->setStretchLastSection(true);
    mPropertiesView->horizontalHeader()->setHighlightSections(false);
    mPropertiesView->horizontalHeader()->setClickable(false);
    mPropertiesView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    QDockWidget* historyDock = new QDockWidget("History", this);
    QUndoView* history = new QUndoView(historyDock);
    historyDock->setWidget(history);
    addDockWidget(Qt::RightDockWidgetArea, historyDock);
}

void EditorWindow::closeEvent(QCloseEvent* closeEvent) {
    QList<MapDocument*>::iterator i;
    for (i = maps_.begin(); i != maps_.end(); ++i) {
        if (!confirmSave(*i)) {
            closeEvent->ignore();
            return;
        }
    }
    closeEvent->accept();
}

void EditorWindow::newMap() {
    NewMapDialog dialog(this);
    Map* map = dialog.createMap();
    if (map) {
        TileSet* tileSet = map->tileSet();

        Tile* tile;
        tile = tileSet->createTile();
        tile->setName("Default Floor");
        tile->setMaterial("SlabFloor");
        tile = tileSet->createTile();
        tile->setName("Default Wall");
        tile->setMaterial("TileWall");
        tile = tileSet->createTile();
        tile->setName("Default Paint");
        tile->setMaterial("BrickWall");

        MapDocument* mapDocument = new MapDocument(map);
        maps_.push_back(mapDocument);
        tabBar_->addTab("Untitled");
        undoGroup_->addStack(mapDocument->getUndoStack());
    }
}

void EditorWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), "", tr("Map Files (*.prj)"));
    if (fileName.isEmpty())
        return;

    MapReader reader;
    Map* map = reader.readMap(fileName);

    if (!map)
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
    else {
        MapDocument* mapDocument = new MapDocument(map);
        mapDocument->setFileName(fileName);
        maps_.push_back(mapDocument);

        QFileInfo fileInfo(fileName);
        tabBar_->addTab(fileInfo.fileName());
        undoGroup_->addStack(mapDocument->getUndoStack());
    }
}

void EditorWindow::save() {
    save(currentMap_);
}

void EditorWindow::saveAs() {
    QMessageBox::critical(this, tr("Error"), tr("Could not write to file."));
}

void EditorWindow::saveAll() {
    QList<MapDocument*>::iterator i;
    for (i = maps_.begin(); i != maps_.end(); ++i) {
        save(*i);
    }
}

void EditorWindow::about() {
    QMessageBox::about(this, tr("About this shit."), tr("The <b>Application</b> example demonstrates how to "
        "write modern GUI applications using Qt, with a menu bar, "
        "toolbars, and a status bar."));
}

void EditorWindow::itemSelected(Item* item) {
    if (currentItem)
        currentItem->setProperties(mPropertiesModel->properties());
    if (item)
        mPropertiesModel->setProperties(item->properties());
    currentItem = item;
}

void EditorWindow::updateStatus(QPoint position) {
    QString message = "X = ";
    message += QString::number(position.x());
    message += ", Y = ";
    message += QString::number(position.y());
    statusBar()->showMessage(message);
}

void EditorWindow::changeTab(int index) {
    if (index < 0) {
        currentMap_ = 0;
        mMapView->setMapDocument(0);
        mMapView->setVisible(false);
        mCatalogueExplorer->clear();
    } else {
        MapDocument* map = maps_.at(index);
        mMapView->setMapDocument(map);
        mCatalogueExplorer->setCatalogue(map->getMap()->catalogue());

        if (!currentMap_) {
            mMapView->setVisible(true);
        }

        if (map->isModified())
            tabBar_->setTabIcon(index, QIcon::fromTheme("document-save"));

        currentMap_ = maps_.at(index);
    }
}

void EditorWindow::closeTab(int index) {
    if (confirmSave(maps_.at(index))) {
        delete maps_.at(index);
        maps_.removeAt(index);
        tabBar_->removeTab(index);
    }
}

void EditorWindow::moveTab(int from, int to) {
    maps_.swap(from, to);
}

bool EditorWindow::confirmSave(MapDocument* mapDocument) {
    if (mapDocument->isModified()) {
        QMessageBox messageBox(this);
        messageBox.setText(tr("The document has been modified."));
        messageBox.setInformativeText(tr("Do you want to save your changes?"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Save);
        messageBox.setEscapeButton(QMessageBox::Cancel);

        int button = messageBox.exec();

        if (button == QMessageBox::Save)
            return save(mapDocument);
        else if (button == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool EditorWindow::save(MapDocument* mapDocument) {
    if (mapDocument->getFileName().isEmpty()) {
        QString fileName = QFileDialog::getSaveFileName(this, "Save caption test", "", tr("Project Files (*.prj)"));

        if (fileName.isEmpty())
            return false;
        else
            mapDocument->setFileName(fileName);
    }
    return mapDocument->save();
}