/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtGui/qwidget.h>

class Tile;
class Type;
class Item;
class Catalogue;
class CategoryModel;
class QToolBox;
class QTreeView;
class QModelIndex;

class CatalogueExplorer : public QWidget {
    Q_OBJECT

public:
    CatalogueExplorer(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~CatalogueExplorer();

    Catalogue* catalogue() const;
    void setCatalogue(Catalogue* catalogue);

    void clear();

signals:
    void tileActivated(Tile* tile);
    void typeActivated(Type* type);
    void itemSelected(Item* item);

private:
    Catalogue* mCatalogue;
    QToolBox* mToolBox;

    QAction* mCreateTileAction;
    QAction* mCreateTypeAction;
    QAction* mCreateCategoryAction;
    QAction* mDestroyItemAction;

    struct {
        QTreeView* view;
        CategoryModel* model;
    } mItemSets[3];

    private slots:
        void createTile();
        void createType();
        void createCategory();
        void destroyItem();

        void showContextMenu(const QPoint& position);

        void itemSelected(const QModelIndex& current);
        void itemActivated(const QModelIndex& index);
};
