/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#ifndef ITEM_H
#define ITEM_H

#include "Property.h"

class Category;

class Item {
public:
    Item();
    virtual ~Item();

    virtual bool isCategory() const;

    Category* toCategory();
    const Category* toCategory() const;

    Category* parent() const;
    void setParent(Category* parent);

    int childNumber();

    QString name() const;
    void setName(QString name);

    PropertyList properties() const;
    void setProperties(PropertyList properties);

protected:
    Category* mParent;

private:
    QString mName;
    PropertyList mProperties;
};

#endif