/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Item.h"
#include <string>

class Category;

class Tile : public Item {
	public:
		Tile(int id, Category* category = 0);
		~Tile();

		int id() const {
			return mId;
		}

		//Category* category() const;

		const std::string& material() const;
		void setMaterial(const std::string& material);

	private:
		int mId;
		//Category* mCategory;
		std::string mMaterial;
};
