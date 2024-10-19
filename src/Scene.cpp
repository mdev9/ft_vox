#include "Scene.hpp"

Scene::Scene(VoxelEngine* engine) {
    chunk = new Chunk(engine);
}

void Scene::update() {
    // Update logic for your scene
	return ;
}

void Scene::render() {
	chunk->render();
}
