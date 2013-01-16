/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#ifndef PROPERTY_H
#define PROPERTY_H

#include <QtCore/qstring.h>
#include <QtCore/qvector.h>

class Property {
	public:
		Property() {
		}

		Property(QString key)
			: mKey(key)
		{
		}

		Property(QString key, QString value)
			: mKey(key)
			, mValue(value)
		{
		}

		QString key() const {
			return mKey;
		}

		void setKey(QString key) {
			mKey = key;
		}

		QString value() const {
			return mValue;
		}

		void setValue(QString value) {
			mValue = value;
		}

		QString operator[](size_t index) {
			return (index == 0) ? mKey : mValue;
		}
	
	private:
		QString mKey;
		QString mValue;
};

typedef QVector<Property> PropertyList;

#endif