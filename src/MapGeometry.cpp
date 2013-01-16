/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "MapGeometry.h"
#include "Map.h"
#include "Region.h"
#include "Tile.h"
#include "TileSet.h"
#include "Type.h"
#include <vector>

MapGeometry::MapGeometry(Ogre::SceneManager* sceneManager)
	: sceneManager_(sceneManager)
	, map_(0)
{
	assert(sceneManager);
	rootNode_ = sceneManager_->getRootSceneNode()->createChildSceneNode();
}

MapGeometry::~MapGeometry() {
	sceneManager_->destroySceneNode(rootNode_);
}

void MapGeometry::setMap(Map* map) {
	if (map_ == map)
		return;
	
	clear();
	map_ = map;

	if (map_ == 0)
		return;

	float x = map->size().x() * 0.5f - 0.5f;
	float z = map->size().y() * 0.5f - 0.5f;

	for (int i = 0; i < map_->size().z(); ++i) {
		Ogre::Vector3 position(x, 1.0f, z);

		Layer layer;

		layer.layerNode = rootNode_->createChildSceneNode();
		layer.layerNode->setPosition(0.0f, i * 2.0f, 0.0f);

		layer.node = layer.layerNode->createChildSceneNode(position);

		layer.region = new Region;
		layer.region->build(map_, i);
		layer.node->attachObject(layer.region);
		
		layers_.push_back(layer);
	}

	for (int i = 0; i < map_->objectCount(); ++i) {
		Object* object = map_->object(i);
		addObject(object);
	}

/*	Ogre::TextureManager* textureManager = Ogre::TextureManager::getSingletonPtr();
	Ogre::String group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

	int width = map->size().x();
	int height = map->size().y();
	int layers = map->size().z();

	mTextures.clear();
	mTextures.reserve(layers);

	for (int k = 0; k < layers; ++k) {
		Ogre::StringStream nameStream;
		nameStream << "Floor" << k;
		Ogre::TexturePtr texture = textureManager->createManual(nameStream.str(), group, Ogre::TEX_TYPE_2D, width, height, 0, Ogre::PF_L8);

		Ogre::HardwarePixelBufferSharedPtr buffer = texture->getBuffer(0, 0);
		const Ogre::PixelBox& box = buffer->lock(Ogre::Image::Box(0, 0, width, height),Ogre::HardwareBuffer::HBL_NORMAL);
		Ogre::uint8* data = static_cast<Ogre::uint8*>(box.data);

		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				Tile* tile = map->cell(Position(i, j, k)).tile(0);
				int id = tile ? tile->id() : 0;
				*data++ = static_cast<Ogre::uint8>(id);
			}
		}
		buffer->unlock();

		mTextures.push_back(texture);
	}

	Ogre::MaterialManager* materialManager = Ogre::MaterialManager::getSingletonPtr();

	TileSet* tileSet = map->tileSet();
	QList<Tile*> tiles = tileSet->tiles();
	int tileCount = tileSet->tiles().size();

	mTileTexture = textureManager->createManual("Tiles", group, Ogre::TEX_TYPE_2D_ARRAY, 128, 128, tileCount, 0, Ogre::PF_R8G8B8);

	int n = 0;
	for (QList<Tile*>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
		Ogre::MaterialPtr material = materialManager->getByName((*it)->material());
		Ogre::String textureName = material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName();
		Ogre::TexturePtr texture = textureManager->createOrRetrieve(textureName, group).first;
		texture->load();
		Ogre::HardwarePixelBufferSharedPtr source = texture->getBuffer(0, 0);
		Ogre::GLTextureBuffer* glSource = static_cast<Ogre::GLTextureBuffer*>(source.get());

		for (size_t i = 0; i <= mTileTexture->getNumMipmaps(); ++i) {
			Ogre::HardwarePixelBufferSharedPtr buffer = mTileTexture->getBuffer(0, i);
			buffer->blit(source, Ogre::Image::Box(0, 0, source->getWidth(), source->getHeight()), Ogre::Image::Box(0, 0, n, buffer->getWidth(), buffer->getHeight(), n + 1));
		}

		++n;
		//if (n == 1) break;
	}

	mMaterials.clear();
	mMaterials.reserve(layers);

	for (int k = 0; k < layers; ++k) {
		Ogre::StringStream nameStream;
		nameStream << "Floor" << k;
		Ogre::MaterialPtr material = materialManager->create(nameStream.str(), group);

		Ogre::Pass* pass = material->getTechnique(0)->getPass(0);
		pass->setFragmentProgram("Tile");
		pass->setLightingEnabled(false);
		pass->setAlphaRejectSettings(Ogre::CMPF_GREATER, 128, true);

		Ogre::TextureUnitState* textureUnit = pass->createTextureUnitState();
		textureUnit->setTexture(mTextures.at(k));
		textureUnit->setTextureFiltering(Ogre::FO_POINT, Ogre::FO_POINT, Ogre::FO_NONE);
		textureUnit->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

		textureUnit = pass->createTextureUnitState();
		textureUnit->setTextureName("Tiles");
		textureUnit->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);

		mMaterials.push_back(material);
	}

	for (std::vector<Ogre::ManualObject*>::iterator it = mGeometry.begin(); it != mGeometry.end(); ++it) {
		sceneManager_->destroyManualObject(*it);
	}
	mGeometry.clear();
	mGeometry.reserve(layers);

	for (int k = 0; k < layers; ++k) {
		Ogre::StringStream nameStream;
		nameStream << "Floor" << k;
		Ogre::ManualObject* manual = sceneManager_->createManualObject(nameStream.str());

		//manual->begin("Grass", Ogre::RenderOperation::OT_TRIANGLE_LIST, group);
		manual->begin(nameStream.str(), Ogre::RenderOperation::OT_TRIANGLE_STRIP, group);
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				manual->position(i * width, k * 2 + 1, j * height);
				manual->normal(0.0f, 1.0f, 0.0f);
				manual->textureCoord(i, j);
			}
		}
		manual->end();

		mGeometry.push_back(manual);
	}

	for (int k = 0; k < layers; ++k) {
		Ogre::SceneNode* node = sceneManager_->getRootSceneNode()->createChildSceneNode();
		node->attachObject(mGeometry.at(k));
	}*/
}

void MapGeometry::setVisibility(float visibility) {
	for (int i = 0; i < map_->size().z(); ++i) {
	}
}

void MapGeometry::updateAll() {
	for (int i = 0; i < map_->size().z(); ++i) {
		layers_[i].region->build(map_, i);
	}
}

void MapGeometry::update(int z) {
	layers_[z].region->build(map_, z);

/*	int width = map_->size().x();
	int height = map_->size().y();
	int layers = map_->size().z();

	Ogre::TexturePtr texture = mTextures.at(z);
	Ogre::HardwarePixelBufferSharedPtr buffer = texture->getBuffer(0, 0);
	const Ogre::PixelBox& box = buffer->lock(Ogre::Image::Box(0, 0, width, height), Ogre::HardwareBuffer::HBL_NORMAL);
	Ogre::uint8* data = static_cast<Ogre::uint8*>(box.data);

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			Tile* tile = map_->cell(Position(i, j, z)).tile(0);
			int id = tile ? tile->id() : 0;
			*data++ = static_cast<Ogre::uint8>(id);
		}
	}
	buffer->unlock();*/
}

void MapGeometry::addObject(Object* object) {
	Layer& layer = layers_.at(object->position().z());

	Ogre::SceneNode* node = layer.layerNode->createChildSceneNode();
	QRect rectangle = object->boundingRectangle();

	Ogre::Vector3 position = Ogre::Vector3(rectangle.left() + rectangle.width() * 0.5f, 0.0f, rectangle.top() + rectangle.height() * 0.5f);
	Ogre::Quaternion orientation(-object->angle().toRadians(), Ogre::Vector3::UNIT_Y);
		
	position += orientation * object->type()->offset();
	orientation = orientation * Ogre::Quaternion(object->type()->rotation(), Ogre::Vector3::UNIT_Y);

	node->setPosition(position);
	node->setOrientation(orientation);
	node->setScale(object->type()->scale(), object->type()->scale(), object->type()->scale());

	Ogre::Entity* entity = sceneManager_->createEntity(object->type()->mesh());
	node->attachObject(entity);

	layer.objects.push_back(entity);
}

void MapGeometry::clear() {
	for (LayerList::iterator i = layers_.begin(); i != layers_.end(); ++i) {
		sceneManager_->destroySceneNode(i->layerNode);
		sceneManager_->destroySceneNode(i->node);
		delete i->region;

		for (QList<Ogre::Entity*>::iterator j = i->objects.begin(); j != i->objects.end(); ++j) {
			sceneManager_->destroyEntity(*j);
		}
	}
	layers_.clear();
}
