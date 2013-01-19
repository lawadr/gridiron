/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Type.h"

Type::Type() {
}

Type::~Type() {
}

const QSize& Type::size() const {
    return mSize;
}

void Type::setSize(const QSize& size) {
    mSize = size;
}

const Ogre::String& Type::mesh() const {
    return mMesh;
}

void Type::setMesh(const Ogre::String& mesh) {
    mMesh = mesh;
}

const Ogre::Vector3& Type::offset() const {
    return mOffset;
}

void Type::setOffset(const Ogre::Vector3& offset) {
    mOffset = offset;
}

Ogre::Radian Type::rotation() const {
    return mRotation;
}

void Type::setRotation(Ogre::Radian rotation) {
    mRotation = rotation;
}

Ogre::Real Type::scale() const {
    return mScale;
}

void Type::setScale(Ogre::Real scale) {
    mScale = scale;
}
