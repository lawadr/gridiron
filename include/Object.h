/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Property.h"
#include "Angle.h"
#include <QtCore/qsize.h>
#include <QtCore/qrect.h>

class Type;

enum Rotation {
	ROTATION_0 = 0,
	ROTATION_90 = 90,
	ROTATION_180 = 180,
	ROTATION_270 = 270
};

class Position {
	public:
		Position()
			: mX(0), mY(0), mZ(0)
		{
		}

		Position(int x, int y, int z)
			: mX(x)	, mY(y)	, mZ(z)
		{
		}

		int x() const {
			return mX;
		}

		int y() const {
			return mY;
		}

		int z() const {
			return mZ;
		}

	private:
		int mX;
		int mY;
		int mZ;
};

class Object {
	public:
		Object(Type* type);
		~Object();

		Type* type() const;

		PropertyList properties() const;
		void setProperties(PropertyList properties);

		const Position& position() const {
			return mPosition;
		}

		void setPosition(const Position& position) {
			mPosition = position;
		}

		Angle angle() const {
			return mAngle;
		}

		void setAngle(Angle angle) {
			mAngle = angle;
		}

		QRect boundingRectangle() const;

	private:
		Type* mType;
		PropertyList mProperties;
		Position mPosition;
		//Rotation mRotation;
		Angle mAngle;
};
