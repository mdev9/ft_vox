#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "BaseMesh.hpp"
#include "QuadMesh.hpp"

class Scene {
public:
    Scene();
    ~Scene();

    void addMesh(BaseMesh* mesh);
    void render();

private:
    std::vector<BaseMesh*> meshes;
};

#endif
