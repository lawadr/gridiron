/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

class Scene;

class SceneItem {
public:
    friend class Scene;

    SceneItem();
    virtual ~SceneItem();

    Scene* scene() const {
        return mScene;
    }

protected:
    Scene* mScene;

private:
    void setScene(Scene* scene) {
        mScene = scene;
    }
};
