/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Item.h"
#include "Category.h"

Item::Item()
    : mParent(0)
{
}

Item::~Item() {
}

bool Item::isCategory() const {
    return false;
}

Category* Item::toCategory() {
    return (isCategory()) ? static_cast<Category*>(this) : 0;
}

const Category* Item::toCategory() const {
    return (isCategory()) ? static_cast<const Category*>(this) : 0;
}

Category* Item::parent() const {
    return mParent;
}

void Item::setParent(Category* parent) {
    mParent = parent;
}

int Item::childNumber() {
    return (mParent) ? mParent->indexOf(this) : 0;
}

QString Item::name() const {
    return mName;
}

void Item::setName(QString name) {
    mName = name;
}

PropertyList Item::properties() const {
    return mProperties;
}

void Item::setProperties(PropertyList properties) {
    mProperties = properties;
}