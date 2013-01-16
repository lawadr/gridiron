/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Item.h"
#include <QtCore/qlist.h>

class Category : public Item {
	public:
		Category();
		~Category();

		bool isCategory() const;
		
		int childCount() const;

		bool isAncestorOf(Item* category) const;
		
		int indexOf(Item* child) const;
		Item* child(int index) const;
		
		void addChild(Item* child);

		Item* removeChild(int index);
		void removeChild(Item* child);

		/*int itemCount() const;

		Item* item(int index) const;

		void addItem(Item* item);

		Item* removeItem(int index);
		void removeItem(Item* item);*/

	private:
		typedef QList<Item*> ChildList;
		//typedef QList<Item*> ItemList;

		ChildList mChildren;
		//ItemList mItems;
};
