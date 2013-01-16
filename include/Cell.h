/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

class Tile;
class Object;

class Cell {
	public:
		Cell() {
			mTiles[0] = 0;
			mTiles[1] = 0;
			mTiles[2] = 0;
			mTiles[3] = 0;
			mTiles[4] = 0;
			mObject = 0;
		}

		bool operator==(const Cell& cell) {
			return (mObject == cell.mObject) && (mTiles[0] == cell.mTiles[0]) && (mTiles[1] == cell.mTiles[1]) && (mTiles[2] == cell.mTiles[2])
				&& (mTiles[3] == cell.mTiles[3]) && (mTiles[4] == cell.mTiles[4]) && (mTiles[5] == cell.mTiles[5]);
		}

		bool operator!=(const Cell& cell) {
			return !(*this == cell);
		}

		bool isEmpty() const {
			return mTiles[0] || mTiles[1] || mTiles[2] || mTiles[3] || mTiles[4];
		}

		Tile* tile(int index) const {
			return mTiles[index];
		}

		void setTile(int index, Tile* tile) {
			mTiles[index] = tile;
		}

		Object* object() const {
			return mObject;
		}

		void setObject(Object* object) {
			mObject = object;
		}
		
	private:
		Tile* mTiles[5];
		Object* mObject;
};
