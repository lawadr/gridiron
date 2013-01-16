/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Item.h"
#include <QtCore/qsize.h>
#include <OGRE/OgreString.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreMath.h>

class Type : public Item {
	public:
		Type();
		~Type();

		const QSize& size() const;
		void setSize(const QSize& size);

		const Ogre::String& mesh() const;
		void setMesh(const Ogre::String& mesh);

		const Ogre::Vector3& offset() const;
		void setOffset(const Ogre::Vector3& offset);

		Ogre::Radian rotation() const;
		void setRotation(Ogre::Radian rotation);

		Ogre::Real scale() const;
		void setScale(Ogre::Real scale);
		
	private:
		QSize mSize;

		Ogre::String mMesh;
		Ogre::Vector3 mOffset;
		Ogre::Radian mRotation;
		Ogre::Real mScale;
};
