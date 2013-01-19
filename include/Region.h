/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <OGRE/Ogre.h>

class Map;
class Tile;
class Cell;

class Region : public Ogre::MovableObject {
public:
    Region();
    Region(const Ogre::String& name);
    ~Region();

    void build(Map* map, int z);
    void clear();

    const Ogre::String& getMovableType() const {
        static Ogre::String type = "Region";
        return type;
    }

    const Ogre::AxisAlignedBox& getBoundingBox() const {
        return boundingBox_;
    }

    Ogre::Real getBoundingRadius() const {
        return boundingRadius_;
    }

    void _updateRenderQueue(Ogre::RenderQueue* queue);
    void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false);

private:
    class MaterialBucket : public Ogre::Renderable {
    public:
        struct Quad1 {
            float x0; float y0; float z0;
            float x1; float y1; float z1;
            float x2; float y2; float z2;
            float x3; float y3; float z3;
            float nx; float ny; float nz;
            float u0; float v0;
            float u1; float v1;
        };

        struct Quad2 {
            Ogre::Vector3 p0;
            Ogre::Vector3 p1;
            Ogre::Vector3 p2;
            Ogre::Vector3 p3;
            Ogre::Vector3 n;
            Ogre::Vector2 t0;
            Ogre::Vector2 t1;

            void fill(float* buffer) const {
                buffer[0] = p0.x;
                buffer[1] = p0.y;
                buffer[2] = p0.z;
                buffer[3] = n.x;
                buffer[4] = n.y;
                buffer[5] = n.z;
                buffer[6] = t0.x;
                buffer[7] = t0.y;

                buffer[8] = p1.x;
                buffer[9] = p1.y;
                buffer[10] = p1.z;
                buffer[11] = n.x;
                buffer[12] = n.y;
                buffer[13] = n.z;
                buffer[14] = t1.x;
                buffer[15] = t0.y;

                buffer[16] = p2.x;
                buffer[17] = p2.y;
                buffer[18] = p2.z;
                buffer[19] = n.x;
                buffer[20] = n.y;
                buffer[21] = n.z;
                buffer[22] = t0.x;
                buffer[23] = t1.y;

                buffer[24] = p3.x;
                buffer[25] = p3.y;
                buffer[26] = p3.z;
                buffer[27] = n.x;
                buffer[28] = n.y;
                buffer[29] = n.z;
                buffer[30] = t1.x;
                buffer[31] = t1.y;
            }
        };

        Ogre::RenderOperation renderOperation;

        MaterialBucket(Region* parent);
        ~MaterialBucket();

        void addPrimitive(float x0, float y0, float z0,
            float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3,
            float nx, float ny, float nz,
            float u0, float v0, float u1, float v1);

        void fillBuffer(float* buffer);
        void fillIndexBuffer(void* buffer);

        const Ogre::MaterialPtr& getMaterial() const {
            return material_;
        }

        void setMaterial(const Ogre::String& material) {
            material_ = Ogre::MaterialManager::getSingleton().getByName(material);
            material_->load();
        }

        void getRenderOperation(Ogre::RenderOperation& operation) {
            operation = renderOperation;
        }

        void getWorldTransforms(Ogre::Matrix4* transform) const {
            transform[0] = parent_->_getParentNodeFullTransform();
        }

        Ogre::Real getSquaredViewDepth(const Ogre::Camera* camera) const {
            return parent_->getParentNode()->getSquaredViewDepth(camera);
        }

        const Ogre::LightList& getLights() const {
            return parent_->queryLights();
        }

    private:
        typedef std::deque<Quad2> QuadList;

        Region* parent_;
        Ogre::MaterialPtr material_;
        QuadList quads_;

        //float* temporaryBuffer_;
        //int temporaryBufferSize_;
    };

    typedef std::map<Tile*, MaterialBucket*> MaterialBucketMap;
    MaterialBucketMap materialBuckets_;

    Ogre::AxisAlignedBox boundingBox_;
    Ogre::Real boundingRadius_;

    void processBottom(Map* map, int z);
    void processSides(char* corners, Map* map, int z);
    void processEdges(int sizeX, int sizeY, char* corners);

    void fillCornerArray(Map* map, int z, char*) const;

    void addPrimitive(float x0, float y0, float z0,
        float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3,
        float nx, float ny, float nz,
        float u0, float v0, float u1, float v1,
        Tile* tile);
};
