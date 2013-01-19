/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "MapView.h"
#include "MapDocument.h"
#include <QtCore/qlist.h>
#include <QtGui/qmainwindow.h>
#include <QtGui/qapplication.h>
#include <QtGui/qlayout.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qtreeview.h>

class PropertiesModel;
class CatalogueExplorer;
class Item;
class QAction;
class QActionGroup;
class QUndoGroup;
class QSortFilterProxyModel;
class QTableView;

class EditorWindow : public QMainWindow {
    Q_OBJECT

public:
    EditorWindow();

protected:
    void closeEvent(QCloseEvent* closeEvent);

private:
    QAction* newAction_;
    QAction* openAction_;
    QAction* saveAction_;
    QAction* saveAsAction_;
    QAction* saveAllAction_;
    QAction* exitAction_;

    QAction* undoAction_;
    QAction* redoAction_;

    QAction* cutAction_;
    QAction* copyAction_;
    QAction* pasteAction_;
    QAction* deleteAction_;

    QAction* gridAction_;
    QAction* upAction_;
    QAction* downAction_;

    QAction* floorAction_;
    QAction* wallAction_;
    QAction* paintAction_;
    QAction* objectAction_;

    QAction* aboutAction_;

    QAction* mAddTileAction;
    QAction* mAddTypeAction;
    QAction* mAddCategoryAction;

    QUndoGroup* undoGroup_;

    MapView* mMapView;

    CatalogueExplorer* mCatalogueExplorer;

    PropertiesModel* mPropertiesModel;
    QTableView* mPropertiesView;

    QDockWidget* explorerDock_;
    QDockWidget* previewDock_;

    QTabBar* tabBar_;

    MapDocument* currentMap_;
    QList<MapDocument*> maps_;

    Item* currentItem;

    void createActions();
    void createMenus();
    void createToolBars();
    void createDocks();

    private slots:
        void newMap();
        void open();
        void save();
        void saveAs();
        void saveAll();

        void about();

        void itemSelected(Item* item);

        void updateStatus(QPoint position);

        void changeTab(int index);
        void closeTab(int index);
        void moveTab(int from, int to);

private:
    bool confirmSave(MapDocument* mapDocument);
    bool save(MapDocument* mapDocument);
};
