/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Category.h"

/*bool comparator(Category* first, Category* second) {
return first->name().toLower() < second->name().toLower();
}*/

Category::Category() {
}

Category::~Category() {
    if (mParent)
        mParent->mChildren.removeAt(childNumber());

    for (ChildList::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
        delete *it;
}

bool Category::isCategory() const {
    return true;
}

/*int Category::childNumber() const {
if (!mParent)
return 0;

ChildList::iterator it = mParent->mChildren.begin();
ChildList::iterator endIt = mParent->mChildren.end();

for (int n = 0; it != endIt; ++n) {
if (*it == this)
return n;
++it;
}

return -1;
}*/

bool Category::isAncestorOf(Item* item) const {
    while (item) {
        if (this == item)
            return true;
        item = item->parent();
    }
    return false;
}

int Category::indexOf(Item* child) const {
    return mChildren.indexOf(child);
}

int Category::childCount() const {
    return mChildren.size();
}

Item* Category::child(int index) const {
    return mChildren.at(index);
}

void Category::addChild(Item* child) {
    if (child->parent() != this) {
        if (child->parent())
            child->parent()->removeChild(child);
        mChildren.push_back(child);
        child->setParent(this);
    }
}

Item* Category::removeChild(int index) {
    Item* child = mChildren.takeAt(index);
    child->setParent(0);
    return child;
}

void Category::removeChild(Item* child) {
    int index = mChildren.indexOf(child);
    if (index != -1)
        removeChild(index);
}

/*int Category::itemCount() const {
return mItems.size();
}

Item* Category::item(int index) const {
return mItems.at(index);
}

void Category::addItem(Item* item) {
if (!mItems.contains(item))
mItems.push_back(item);
}

Item* Category::removeItem(int index) {
Item* item = mItems.takeAt(index);
return item;
}

void Category::removeItem(Item* item) {
int index = mItems.indexOf(item);
if (index != -1)
removeItem(index);
}*/