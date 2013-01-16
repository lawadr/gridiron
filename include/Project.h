/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtCore/qobject.h>
#include <QtCore/qvector.h>
#include <QtXml/qxmlstream.h>

class World;

struct FloorType2 {
	QString name;
	QString material;

	void read(QXmlStreamReader& stream) {
		stream.readNextStartElement();
		if (stream.qualifiedName() == "Name") {
			name = stream.readElementText();
		} else if (stream.qualifiedName() == "Material") {
			material = stream.readElementText();
		}
	}

	void write(QXmlStreamWriter& stream) const {
		stream.writeTextElement("Name", name);
		stream.writeTextElement("Material", material);

		stream.writeEndElement();
	}
};

struct WallType2 {
	QString name;
	QString material;

	void read(QXmlStreamReader& stream) {
		stream.readNextStartElement();
		if (stream.qualifiedName() == "Name") {
			name = stream.readElementText();
		} else if (stream.qualifiedName() == "Material") {
			material = stream.readElementText();
		}
	}

	void write(QXmlStreamWriter& stream) const {
		stream.writeStartElement("Wall");

		stream.writeTextElement("Name", name);
		stream.writeTextElement("Material", material);

		stream.writeEndElement();
	}
};

struct ObjectType2 {
	QString name;
	QString mesh;

	void read(QXmlStreamReader& stream) {
		stream.readNextStartElement();
		if (stream.qualifiedName() == "Name") {
			name = stream.readElementText();
		} else if (stream.qualifiedName() == "Mesh") {
			mesh = stream.readElementText();
		}
	}

	void write(QXmlStreamWriter& stream) const {
		stream.writeStartElement("Object");

		stream.writeTextElement("Name", name);
		stream.writeTextElement("Mesh", mesh);

		stream.writeEndElement();
	}
};

class Project2 {
	public:
		Project2();
		~Project2();

		bool open(const QString& filename);
		bool save();
		void close();

		bool isOpen();

	private:
		QString name_;
		QString location_;
		QString fileName_;

		QVector<FloorType2> floorTypes_;
		QVector<WallType2> wallTypes_;
		QVector<ObjectType2> objectTypes_;

		World* world_;

		void loadTypes();
		void saveTypes();
};
