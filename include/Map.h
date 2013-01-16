/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Cell.h"
#include "Object.h"
#include "Property.h"
#include "Catalogue.h"
#include <QtCore/qvector.h>

class Type;

class Size {
	public:
		Size()
			: mX(0), mY(0), mZ(0)
		{
		}

		Size(int x, int y, int z)
			: mX(x)	, mY(y)	, mZ(z)
		{
		}

		int x() const {
			return mX;
		}

		void setX(int x) {
			mX = x;
		}

		int y() const {
			return mY;
		}

		void setY(int y) {
			mY = y;
		}

		int z() const {
			return mZ;
		}

		void setZ(int z) {
			mZ = z;
		}

	private:
		int mX;
		int mY;
		int mZ;
};

class Map {
	public:
		Map(const Size& size);
		~Map();

		TileSet* tileSet() const {
			return mCatalogue->tileSet();
		}

		TypeSet* typeSet() const {
			return mCatalogue->typeSet();
		}

		Catalogue* catalogue() const {
			return mCatalogue;
		}

		PropertyList properties() const {
			return mProperties;
		}

		void setProperties(PropertyList properties) {
			mProperties = properties;
		}
		
		const Size& size() const {
			return mSize;
		}

		const Cell& cell(const Position& position) const {
			return mCells.at(cellIndex(position));
		}

		void setCell(const Position& position, const Cell& cell) {
			mCells.replace(cellIndex(position), cell);
		}

		bool hasWall(const Position& position, bool eastWestFacing) const {
			bool singlePosition = false;
			int face;
			Position positions[2];

			if (eastWestFacing) {
				face = 2;
				
				positions[0] = Position(position.x() - 1, position.y(), position.z());
				positions[1] = position;

				if (position.x() <= 0) {
					face = 4;
					positions[0] = positions[1];
					singlePosition = true;
				} else if (position.x() >= mSize.x()) {
					singlePosition = true;
				}
			} else {
				face = 1;

				positions[0] = position;
				positions[1] = Position(position.x(), position.y() - 1, position.z());

				if (position.y() <= 0) {
					singlePosition = true;
				} else if (position.y() >= mSize.y()) {
					face = 3;
					positions[0] = positions[1];
					singlePosition = true;
				}
			}

			if (cell(positions[0]).tile(face) != 0)
				return true;

			if (!singlePosition && (cell(positions[1]).tile(face + 2) != 0))
				return true;
			
			return false;
		}

		Object* createObject(Type* type);
		void destroyObject(Object* object);

		Object* object(int index) const;
		int objectCount() const;

	private:
		Size mSize;
		QVector<Cell> mCells;
		QVector<Object*> mObjects;
		PropertyList mProperties;

		Catalogue* mCatalogue;

		int cellIndex(const Position& position) const {
			return position.x() + position.y() * mSize.x() + position.z() * mSize.x() * mSize.y();
		}

		//QVector<QVector<Wall>> mWallsX;
		//QVector<QVector<Wall>> mWallsY;
};
