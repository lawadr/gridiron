/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "MapDocument.h"
#include "Map.h"
#include "MapWriter.h"
#include <QtGui/qundostack.h>
#include <QtCore/qfile.h>

MapDocument::MapDocument()
    : map_(0)
    , isModified_(true)
{
    undoStack_ = new QUndoStack(this);
}

MapDocument::MapDocument(Map* map)
    : map_(map)
    , isModified_(true)
{
    undoStack_ = new QUndoStack(this);
}

MapDocument::~MapDocument() {
    if (map_)
        delete map_;
}

QUndoStack* MapDocument::getUndoStack() const {
    return undoStack_;
}

Map* MapDocument::getMap() const {
    return map_;
}

bool MapDocument::isModified() const {
    return isModified_;
}

QString MapDocument::getFileName() const {
    return fileName_;
}

void MapDocument::setFileName(QString fileName) {
    fileName_ = fileName;
}

bool MapDocument::save() {
    if (!fileName_.isEmpty()) {
        MapWriter writer;
        writer.write(map_, fileName_);

        //QFile file(fileName_);
        //if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //	return false;
        //}
        //file.write("Test Data");
        //file.close();
        return true;
    } else
        return false;
}