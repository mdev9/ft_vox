#include "Scene.hpp"
#include "ShaderProgram.hpp"

#include <iostream> //debug

Scene::Scene() {}

Scene::~Scene() {
    for (BaseMesh* mesh : meshes) {
        delete mesh;
    }
}

void Scene::addMesh(BaseMesh* mesh) {
    meshes.push_back(mesh);
}

void Scene::render() {
	int i = 0;
    for (BaseMesh* mesh : meshes) {
		i++;
        mesh->draw();
    }
	std::cout << "rendered " << i << "meshes\n";
}
