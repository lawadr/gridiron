/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include "Region.h"
#include "Map.h"
#include "Tile.h"

Tile* defaultTile;

Region::Region()
    : boundingBox_(Ogre::AxisAlignedBox::BOX_NULL)
    , boundingRadius_(0.0f)
{
    defaultTile = new Tile(0);
    defaultTile->setName("Default Top Tile");
    defaultTile->setMaterial("UniformBlack");
}

Region::Region(const Ogre::String& name)
    : Ogre::MovableObject(name)
    , boundingBox_(Ogre::AxisAlignedBox::BOX_NULL)
    , boundingRadius_(0.0f)
{
}

Region::~Region() {
    clear();
}

void Region::build(Map* map, int z) {
    clear();

    char* corners = new char[(map->size().x() + 1) * (map->size().y() + 1)];
    fillCornerArray(map, z, corners);

    processBottom(map, z);
    processSides(corners, map, z);
    processEdges(map->size().x() + 1, map->size().y() + 1, corners);

    delete[] corners;

    size_t vertexCount = 0;
    size_t indexCount = 0;
    for (MaterialBucketMap::iterator i = materialBuckets_.begin(); i != materialBuckets_.end(); ++i) {
        i->second->renderOperation.vertexData->vertexStart = vertexCount;
        i->second->renderOperation.indexData->indexStart = indexCount;

        vertexCount += i->second->renderOperation.vertexData->vertexCount;
        indexCount += i->second->renderOperation.indexData->indexCount;
    }

    Ogre::HardwareBufferManager& bufferManager = Ogre::HardwareBufferManager::getSingleton();

    size_t vertexSize = Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3) * 2 + Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
    Ogre::HardwareVertexBufferSharedPtr vertexBuffer = bufferManager.createVertexBuffer(vertexSize, vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    Ogre::HardwareIndexBuffer::IndexType indexType = (vertexCount >= 65536) ? Ogre::HardwareIndexBuffer::IT_32BIT : Ogre::HardwareIndexBuffer::IT_16BIT;
    Ogre::HardwareIndexBufferSharedPtr indexBuffer = bufferManager.createIndexBuffer(indexType, indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    float* vertexBufferSpace = static_cast<float*>(vertexBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
    char* indexBufferSpace = static_cast<char*>(indexBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));

    for (MaterialBucketMap::iterator i = materialBuckets_.begin(); i != materialBuckets_.end(); ++i) {
        i->second->renderOperation.indexData->indexBuffer = indexBuffer;
        i->second->renderOperation.vertexData->vertexBufferBinding->setBinding(0, vertexBuffer);

        i->second->fillBuffer(vertexBufferSpace);
        vertexBufferSpace += (i->second->renderOperation.vertexData->vertexCount * 8);

        i->second->fillIndexBuffer(indexBufferSpace);
        indexBufferSpace += (i->second->renderOperation.indexData->indexCount * indexBuffer->getIndexSize());
    }

    vertexBuffer->unlock();
    indexBuffer->unlock();

    Ogre::Real sizeX = map->size().x() * 0.5f;
    Ogre::Real sizeY = map->size().y() * 0.5f;

    boundingBox_.setExtents(-sizeX, -1.0f, -sizeY, sizeX, 1.0f, sizeY);
    boundingRadius_ = Ogre::Math::Sqrt(sizeX * sizeX + sizeY * sizeY + 1.0f);
}

void Region::clear() {
    boundingRadius_ = 0.0f;
    boundingBox_.setNull();

    for (MaterialBucketMap::iterator i = materialBuckets_.begin(); i != materialBuckets_.end(); ++i)
        delete i->second;

    materialBuckets_.clear();
}

void Region::_updateRenderQueue(Ogre::RenderQueue* queue) {
    for (MaterialBucketMap::iterator i = materialBuckets_.begin(); i != materialBuckets_.end(); ++i)
        queue->addRenderable(i->second);
}

void Region::visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables) {
    for (MaterialBucketMap::iterator i = materialBuckets_.begin(); i != materialBuckets_.end(); ++i)
        visitor->visit(i->second, 0, debugRenderables);
}

void Region::processBottom(Map* map, int z) {
    int sizeX = map->size().x();
    int sizeY = map->size().y();

    float halfSizeX = sizeX * 0.5f;
    float halfSizeY = sizeY * 0.5f;

    char* marks = new char[sizeX * sizeY];
    memset(marks, 0, sizeX * sizeY);

    for (int j = 0; j < sizeY; ++j) {
        for (int i = 0; i < sizeX; ++i) {
            if (marks[i + j * sizeX])
                continue;

            Tile* tile = map->cell(Position(i, j, z)).tile(0);
            if (!tile)
                continue;

            int x = i;
            int y = j;

            bool skipX = (i + 1) == sizeX;
            bool skipY = (j + 1) == sizeY;

            while (!skipX || !skipY) {
                if (!skipX) {
                    for (int jn = j; jn <= y; ++jn) {
                        if ((tile != map->cell(Position(x + 1, jn, z)).tile(0)) || (marks[(x + 1) + jn * sizeX])) {
                            skipX = true;
                            break;
                        }
                    }
                    if (!skipX) {
                        ++x;
                        skipX = (x + 1) == sizeX;
                    }
                }

                if (!skipY) {
                    for (int in = i; in <= x; ++in) {
                        if ((tile != map->cell(Position(in, y + 1, z)).tile(0)) || (marks[in + (y + 1) * sizeX])) {
                            skipY = true;
                            break;
                        }
                    }
                    if (!skipY) {
                        ++y;
                        skipY = (y + 1) == sizeY;
                    }
                }
            }

            for (int jn = j; jn <= y; ++jn)
                for (int in = i; in <= x; ++in)
                    marks[in + jn * sizeX] = 1;

            addPrimitive(i - halfSizeX, -1.0f, j - halfSizeY, 
                (x + 1.0f) - halfSizeX, -1.0f, j - halfSizeY, 
                i - halfSizeX, -1.0f, (y + 1.0f) - halfSizeY, 
                (x + 1.0f) - halfSizeX, -1.0f, (y + 1.0f) - halfSizeY, 
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, (x + 1.0f) - i, (y + 1.0f) - j, tile);
        }
    }

    delete[] marks;
}

void Region::processSides(char* corners, Map* map, int z) {
    int sizeX = map->size().x();
    int sizeY = map->size().y();

    float halfSizeX = sizeX * 0.5f;
    float halfSizeY = sizeY * 0.5f;

    int start[2];
    char startTruncated[2];
    char endTruncated[2];

    Tile* previousTile[2];

    for (int j = 0; j < sizeY; ++j) {
        previousTile[0] = 0;
        previousTile[1] = 0;
        for (int i = 0; i < sizeX; ++i) {
            const Cell& cell = map->cell(Position(i, j, z));

            if (cell.tile(1) && ((cell.tile(1) != previousTile[0]) || (corners[i + j * (map->size().x() + 1)] & 4))) {
                start[0] = i;
                startTruncated[0] = (corners[i + j * (map->size().x() + 1)] & 4) ? 1 : ((previousTile[0]) ? 0 : -1);
            }

            if (cell.tile(1) && ((corners[(i + 1) + j * (map->size().x() + 1)] & 4) || ((i + 1) == map->size().x()) || (cell.tile(1) != map->cell(Position(i + 1, j, z)).tile(1)))) {
                endTruncated[0] = (corners[(i + 1) + j * (map->size().x() + 1)] & 4) ? 1 : ((corners[(i + 1) + j * (map->size().x() + 1)] & 2) ? 0 : -1);

                addPrimitive(start[0] + (0.1f * startTruncated[0]) - halfSizeX, 1.0f, (j + 0.1f) - halfSizeY, 
                    (i + 1.0f) - (0.1f * endTruncated[0]) - halfSizeX, 1.0f, (j + 0.1f) - halfSizeY, 
                    start[0] + (0.1f * startTruncated[0]) - halfSizeX, -1.0f, (j + 0.1f) - halfSizeY, 
                    (i + 1.0f) - (0.1f * endTruncated[0]) - halfSizeX, -1.0f, (j + 0.1f) - halfSizeY, 
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, (i + 1.0f) - start[0], 1.0f, cell.tile(1));
            }

            if (cell.tile(3) && ((cell.tile(3) != previousTile[1]) || (corners[i + (j + 1) * (map->size().x() + 1)] & 1))) {
                start[1] = i;
                startTruncated[1] = (corners[i + (j + 1) * (map->size().x() + 1)] & 1) ? 1 : ((previousTile[1]) ? 0 : -1);
            }

            if (cell.tile(3) && ((corners[(i + 1) + (j + 1) * (map->size().x() + 1)] & 1) || ((i + 1) == map->size().x()) || (cell.tile(3) != map->cell(Position(i + 1, j, z)).tile(3)))) {
                endTruncated[1] = (corners[(i + 1) + (j + 1) * (map->size().x() + 1)] & 1) ? 1 : ((corners[(i + 1) + (j + 1) * (map->size().x() + 1)] & 2) ? 0 : -1);

                addPrimitive((i + 1.0f) - (0.1f * endTruncated[1]) - halfSizeX, 1.0f, (j + 0.9f) - halfSizeY, 
                    start[1] + (0.1f * startTruncated[1]) - halfSizeX, 1.0f, (j + 0.9f) - halfSizeY, 
                    (i + 1.0f) - (0.1f * endTruncated[1]) - halfSizeX, -1.0f, (j + 0.9f) - halfSizeY, 
                    start[1] + (0.1f * startTruncated[1]) - halfSizeX, -1.0f, (j + 0.9f) - halfSizeY, 
                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, (i + 1.0f) - start[1], 1.0f, cell.tile(3));
            }

            previousTile[0] = cell.tile(1);
            previousTile[1] = cell.tile(3);
        }
    }

    for (int i = 0; i < sizeX; ++i) {
        previousTile[0] = 0;
        previousTile[1] = 0;
        for (int j = 0; j < sizeY; ++j) {
            const Cell& cell = map->cell(Position(i, j, z));

            if (cell.tile(4) && ((cell.tile(4) != previousTile[0]) || (corners[i + j * (map->size().x() + 1)] & 2))) {
                start[0] = j;
                startTruncated[0] = (corners[i + j * (map->size().x() + 1)] & 2) ? 1 : ((previousTile[0]) ? 0 : -1);
            }

            if (cell.tile(4) && ((corners[i + (j + 1) * (map->size().x() + 1)] & 2) || ((j + 1) == map->size().y()) || (cell.tile(4) != map->cell(Position(i, j + 1, z)).tile(4)))) {
                endTruncated[0] = (corners[i + (j + 1) * (map->size().x() + 1)] & 2) ? 1 : ((corners[i + (j + 1) * (map->size().x() + 1)] & 4) ? 0 : -1);

                addPrimitive((i + 0.1f) - halfSizeX, 1.0f, (j + 1.0f) - (0.1f * endTruncated[0]) - halfSizeY, 
                    (i + 0.1f) - halfSizeX, 1.0f, start[0] + (0.1f * startTruncated[0]) - halfSizeY, 
                    (i + 0.1f) - halfSizeX, -1.0f, (j + 1.0f) - (0.1f * endTruncated[0]) - halfSizeY, 
                    (i + 0.1f) - halfSizeX, -1.0f, start[0] + (0.1f * startTruncated[0]) - halfSizeY, 
                    1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, (j + 1.0f) - start[0], 1.0f, cell.tile(4));
            }

            if (cell.tile(2) && ((cell.tile(2) != previousTile[1]) || (corners[(i + 1) + j * (map->size().x() + 1)] & 8))) {
                start[1] = j;
                startTruncated[1] = (corners[(i + 1) + j * (map->size().x() + 1)] & 8) ? 1 : ((previousTile[1]) ? 0 : -1);
            }

            if (cell.tile(2) && ((corners[(i + 1) + (j + 1) * (map->size().x() + 1)] & 8) || ((j + 1) == map->size().y()) || (cell.tile(2) != map->cell(Position(i, j + 1, z)).tile(2)))) {
                endTruncated[1] = (corners[(i + 1) + (j + 1) * (map->size().x() + 1)] & 8) ? 1 : ((corners[(i + 1) + (j + 1) * (map->size().x() + 1)] & 4) ? 0 : -1);

                addPrimitive((i + 0.9f) - halfSizeX, 1.0f, start[1] + (0.1f * startTruncated[1]) - halfSizeY, 
                    (i + 0.9f) - halfSizeX, 1.0f, (j + 1.0f) - (0.1f * endTruncated[1]) - halfSizeY, 
                    (i + 0.9f) - halfSizeX, -1.0f, start[1] + (0.1f * startTruncated[1]) - halfSizeY, 
                    (i + 0.9f) - halfSizeX, -1.0f, (j + 1.0f) - (0.1f * endTruncated[1]) - halfSizeY, 
                    -1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, (j + 1.0f) - start[1], 1.0f, cell.tile(2));
            }

            previousTile[0] = cell.tile(4);
            previousTile[1] = cell.tile(2);
        }
    }
}

void Region::processEdges(int sizeX, int sizeY, char* corners) {
    int start;
    char startTruncated;
    char endTruncated;

    float halfSizeX = (sizeX - 1) * 0.5f;
    float halfSizeY = (sizeY - 1) * 0.5f;

    for (int j = 0; j < sizeY; ++j) {
        for (int i = 0; i < sizeX; ++i) {
            char corner = corners[i + j * sizeX];
            if ((corner & 10) == 2) {
                startTruncated = ((corner & 5) == 5) ? 1 : -1;
                start = i;
            } else if ((corner & 10) == 8) {
                endTruncated = ((corner & 5) == 5) ? 1 : -1;

                addPrimitive(start + (startTruncated * 0.1f) - halfSizeX, 1.0f, (j - 0.1f) - halfSizeY,
                    i - (endTruncated * 0.1f) - halfSizeX, 1.0f, (j - 0.1f) - halfSizeY,
                    start + (startTruncated * 0.1f) - halfSizeX, 1.0f, (j + 0.1f) - halfSizeY,
                    i - (endTruncated * 0.1f) - halfSizeX, 1.0f, (j + 0.1f) - halfSizeY,
                    0.0f, 1.0f, 0.0f,
                    startTruncated * 0.1f, -0.1f, (i - start) - (endTruncated * 0.1f), 0.1f, defaultTile);
            }
        }
    }

    for (int i = 0; i < sizeX; ++i) {
        for (int j = 0; j < sizeY; ++j) {
            char corner = corners[i + j * sizeX];
            if ((corner & 5) == 4) {
                startTruncated = (corner & 10) ? 1 : -1;
                start = j;
            } else if ((corner == 15) || ((corner & 5) == 1)) {
                endTruncated = (corner & 10) ? 1 : -1;

                addPrimitive((i - 0.1f) - halfSizeX, 1.0f, start + (startTruncated * 0.1f) - halfSizeY,
                    (i + 0.1f) - halfSizeX, 1.0f, start + (startTruncated * 0.1f) - halfSizeY,
                    (i - 0.1f) - halfSizeX, 1.0f, j - (endTruncated * 0.1f) - halfSizeY,
                    (i + 0.1f) - halfSizeX, 1.0f, j - (endTruncated * 0.1f) - halfSizeY,
                    0.0f, 1.0f, 0.0f,
                    -0.1f, -0.1f, 0.1f, (j - start) + 0.1f, defaultTile);

                if (corner == 15) {
                    startTruncated = true;
                    start = j;
                }
            }
        }
    }
}

void Region::fillCornerArray(Map* map, int z, char* corners) const {
    memset(corners, 0, (map->size().x() + 1) * (map->size().y() + 1));

    for (int j = 0; j < map->size().y(); ++j) {
        for (int i = 0; i < map->size().x(); ++i) {
            const Cell& cell = map->cell(Position(i, j, z));
            if (cell.tile(1)) {
                corners[i + j * (map->size().x() + 1)] |= 2;
                corners[(i + 1) + j * (map->size().x() + 1)] |= 8;
            }
            if (cell.tile(2)) {
                corners[(i + 1) + j * (map->size().x() + 1)] |= 4;
                corners[(i + 1) + (j + 1) * (map->size().x() + 1)] |= 1;
            }
            if (cell.tile(3)) {
                corners[(i + 1) + (j + 1) * (map->size().x() + 1)] |= 8;
                corners[i + (j + 1) * (map->size().x() + 1)] |= 2;
            }
            if (cell.tile(4)) {
                corners[i + (j + 1) * (map->size().x() + 1)] |= 1;
                corners[i + j * (map->size().x() + 1)] |= 4;
            }
        }
    }
}

void Region::addPrimitive(float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float nx, float ny, float nz,
    float u0, float v0, float u1, float v1,
    Tile* tile)
{
    MaterialBucketMap::const_iterator i = materialBuckets_.find(tile);
    if (i == materialBuckets_.end()) {
        MaterialBucket* bucket = new MaterialBucket(this);
        bucket->setMaterial(tile->material());
        i = materialBuckets_.insert(std::make_pair(tile, bucket)).first;
    }
    i->second->addPrimitive(x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3, nx, ny, nz, u0, v0, u1, v1);
}

//---------------------------------------------------------------------------------------

Region::MaterialBucket::MaterialBucket(Region* parent)
    : parent_(parent)
{
    renderOperation.srcRenderable = this;
    renderOperation.useIndexes = true;
    renderOperation.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

    renderOperation.vertexData = new Ogre::VertexData;
    renderOperation.vertexData->vertexCount = 0;
    size_t offset = renderOperation.vertexData->vertexDeclaration->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION).getSize();
    offset += renderOperation.vertexData->vertexDeclaration->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL).getSize();
    renderOperation.vertexData->vertexDeclaration->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

    renderOperation.indexData = new Ogre::IndexData;
    renderOperation.indexData->indexCount = 0;
}

Region::MaterialBucket::~MaterialBucket() {
    delete renderOperation.vertexData;
    delete renderOperation.indexData;
}

void Region::MaterialBucket::addPrimitive(float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float nx, float ny, float nz,
    float u0, float v0, float u1, float v1)
{
    quads_.push_back(Quad2());
    Quad2& quad = quads_.back();

    quad.p0 = Ogre::Vector3(x0, y0, z0);
    quad.p1 = Ogre::Vector3(x1, y1, z1);
    quad.p2 = Ogre::Vector3(x2, y2, z2);
    quad.p3 = Ogre::Vector3(x3, y3, z3);
    quad.n = Ogre::Vector3(nx, ny, nz);
    quad.t0 = Ogre::Vector2(u0, v0);
    quad.t1 = Ogre::Vector2(u1, v1);

    renderOperation.vertexData->vertexCount += 4;
    renderOperation.indexData->indexCount += 6;
}

void Region::MaterialBucket::fillBuffer(float* buffer) {
    for (QuadList::const_iterator i = quads_.begin(); i != quads_.end(); ++i) {
        i->fill(buffer);
        buffer += 32;
    }
}

void Region::MaterialBucket::fillIndexBuffer(void* buffer) {
    if (renderOperation.indexData->indexBuffer->getType() == Ogre::HardwareIndexBuffer::IT_16BIT) {
        Ogre::uint16* mbuffer = static_cast<Ogre::uint16*>(buffer);
        for (size_t i = 0; i < (renderOperation.indexData->indexCount / 6); ++i) {
            mbuffer[(i * 6)] = i * 4;
            mbuffer[(i * 6) + 1] = i * 4 + 2;
            mbuffer[(i * 6) + 2] = i * 4 + 3;
            mbuffer[(i * 6) + 3] = i * 4;
            mbuffer[(i * 6) + 4] = i * 4 + 3;
            mbuffer[(i * 6) + 5] = i * 4 + 1;
        }
    } else {
        Ogre::uint32* mbuffer = static_cast<Ogre::uint32*>(buffer);
        for (size_t i = 0; i < (renderOperation.indexData->indexCount / 6); ++i) {
            mbuffer[(i * 6)] = i * 4;
            mbuffer[(i * 6) + 1] = i * 4 + 2;
            mbuffer[(i * 6) + 2] = i * 4 + 3;
            mbuffer[(i * 6) + 3] = i * 4;
            mbuffer[(i * 6) + 4] = i * 4 + 3;
            mbuffer[(i * 6) + 5] = i * 4 + 1;
        }
    }
}