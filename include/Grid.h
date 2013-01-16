/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <OGRE/Ogre.h>

class Grid : public Ogre::RenderTargetListener {
	public:
		Grid(Ogre::SceneManager* sceneManager, Ogre::Viewport* viewport);
		~Grid();

		Ogre::SceneManager* getSceneManager() const;
		Ogre::Viewport* getViewport() const;

		const Ogre::ColourValue& getColour() const;
		void setColour(const Ogre::ColourValue& colour);

		bool isVisible() const;
		void setVisible(bool visible);

		size_t getSizeX() const;
		void setSizeX(size_t size);

		size_t getSizeZ() const;
		void setSizeZ(size_t size);

		void setSize(size_t sizeX, size_t sizeZ);

		Ogre::Real getElevation() const;
		void setElevation(Ogre::Real elevation);
		
		void preViewportUpdate(const Ogre::RenderTargetViewportEvent& viewportEvent);
		void postViewportUpdate(const Ogre::RenderTargetViewportEvent& viewportEvent);

	private:
		static const Ogre::String MATERIAL_NAME;

		Ogre::SceneManager* sceneManager_;
		Ogre::Viewport* viewport_;

		Ogre::SceneNode* sceneNode_;
		Ogre::ManualObject* lines_;

		size_t sizeX_;
		size_t sizeZ_;
		
		Ogre::ColourValue colour_;
		bool visible_;

		void resizeGeometry();
};

class MovableGrid : public Ogre::SimpleRenderable {
	public:
		MovableGrid();
		MovableGrid(const Ogre::String& name);
		~MovableGrid();

		const Ogre::String& getMovableType() const;

		Ogre::Real getBoundingRadius() const;
		Ogre::Real getSquaredViewDepth(const Ogre::Camera* camera) const;

		const Ogre::ColourValue& colour() const;
		void setColour(const Ogre::ColourValue& colour);

		void getSize(size_t& x, size_t& y) const;
		void setSize(size_t x, size_t y);

	private:
		size_t mX;
		size_t mY;
		Ogre::ColourValue mColour;

		Ogre::Real mBoundingRadius;

		void init();
		void update();
};

class MovableGridFactory : public Ogre::MovableObjectFactory {
	public:
		const Ogre::String& getType() const {
			static Ogre::String type = "MovableGrid";
			return type;
		}

		void destroyInstance(Ogre::MovableObject* object) {
			delete object;
		}

	protected:
		Ogre::MovableObject* createInstanceImpl(const Ogre::String& name, const Ogre::NameValuePairList* parameters) {
			return new MovableGrid(name);
		}
};
