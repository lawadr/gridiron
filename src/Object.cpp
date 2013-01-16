/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Object.h"
#include "Type.h"
#include "Angle.h"
#include <QtCore/qpoint.h>

Object::Object(Type* type)
	: mType(type)
	//, mRotation(ROTATION_0)
{
}

Object::~Object() {
}

Type* Object::type() const {
	return mType;
}

PropertyList Object::properties() const {
	return mProperties;
}

void Object::setProperties(PropertyList properties) {
	mProperties = properties;
}

QRect Object::boundingRectangle() const {
	const QSize& size = mType->size();

	QPoint point1(-((size.width() - 1) >> 1), -((size.height() - 1) >> 1));
	QPoint point2(size.width() >> 1, size.height() >> 1);

	//Angle angle(mRotation / 90);
	mAngle.rotate(point1);
	mAngle.rotate(point2);

	QRect rectangle;
	rectangle.setLeft(std::min(point1.x(), point2.x()));
	rectangle.setTop(std::min(point1.y(), point2.y()));
	rectangle.setWidth(abs(point1.x()) + abs(point2.x()));
	rectangle.setHeight(abs(point1.y()) + abs(point2.y()));

	rectangle.translate(mPosition.x(), mPosition.y());

	return rectangle;
}