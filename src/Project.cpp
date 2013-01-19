/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Project.h"
#include <QtCore/qdir.h>
#include <QtCore/qtextstream.h>
#include <QtXml/qdom.h>

Project2::Project2() {
}

Project2::~Project2() {
}

bool Project2::open(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QXmlStreamReader stream(&file);
    if (stream.readNextStartElement()) {
        if (stream.qualifiedName() == "Project") {
            if (stream.qualifiedName() == "Name") {
                name_ = stream.readElementText();
            }
        }
    }

    close();

    fileName_ = file.fileName();
    location_ = file.fileName();

    loadTypes();

    return true;
}

bool Project2::save() {
    QDir directory(location_);
    QFile file(directory.absoluteFilePath(fileName_));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamWriter stream(&file);
    stream.writeStartDocument();
    stream.writeStartElement("Project");
    stream.writeTextElement("Name", name_);
    stream.writeEndElement();
    stream.writeEndElement();
    stream.writeEndDocument();

    file.close();

    return true;
}

void Project2::close() {
    name_.clear();
    location_.clear();
    fileName_.clear();

    floorTypes_.clear();
    wallTypes_.clear();
    objectTypes_.clear();
    //world_.clear();
}

void Project2::loadTypes() {
    QFile file(QDir(location_).filePath("Types.xml"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QXmlStreamReader stream(&file);
    if (stream.readNextStartElement()) {
        if (stream.qualifiedName() == "Types") {
            while (stream.readNextStartElement()) {
                if (stream.qualifiedName() == "Floor") {
                    FloorType2 type;
                    type.read(stream);
                    floorTypes_.push_back(type);
                } else if (stream.qualifiedName() == "Wall") {
                    WallType2 type;
                    type.read(stream);
                    wallTypes_.push_back(type);
                } else if (stream.qualifiedName() == "Object") {
                    ObjectType2 type;
                    type.read(stream);
                    objectTypes_.push_back(type);
                }
            }
        }
    }

    file.close();
}

void Project2::saveTypes() {
    QFile file(QDir(location_).filePath("Types.xml"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QXmlStreamWriter stream(&file);
    stream.writeStartDocument();
    stream.writeStartElement("Types");

    QVector<FloorType2>::const_iterator floorTypeIt;
    for (floorTypeIt = floorTypes_.begin(); floorTypeIt != floorTypes_.begin(); ++floorTypeIt) {
        stream.writeStartElement("Floor");
        floorTypeIt->write(stream);
        stream.writeEndElement();
    }

    QVector<WallType2>::const_iterator wallTypeIt;
    for (wallTypeIt = wallTypes_.begin(); wallTypeIt != wallTypes_.begin(); ++wallTypeIt) {
        stream.writeStartElement("Wall");
        wallTypeIt->write(stream);
        stream.writeEndElement();
    }

    QVector<ObjectType2>::const_iterator objectTypeIt;
    for (objectTypeIt = objectTypes_.begin(); objectTypeIt != objectTypes_.begin(); ++objectTypeIt) {
        stream.writeStartElement("Object");
        objectTypeIt->write(stream);
        stream.writeEndElement();
    }

    stream.writeEndElement();
    stream.writeEndDocument();

    file.close();
}
