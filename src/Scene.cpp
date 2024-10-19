#include "Scene.hpp"

Scene::Scene(VoxelEngine* engine) {
    addMesh(new QuadMesh(engine->shaderProgram));
}

Scene::~Scene() {
    for (BaseMesh* mesh : meshes) {
        delete mesh;
    }
}

void Scene::addMesh(BaseMesh* mesh) {
    meshes.push_back(mesh);
}

void Scene::update() {
    // Update logic for your scene
	return ;
}

void Scene::render() {
	for (BaseMesh* mesh : meshes) {
		mesh->render();
	}
}
