#include "Scene.hpp"

Scene::Scene(VoxelEngine* engine) {
    world = new World(engine);
}

void Scene::update() {
	world->update();
	return ;
}

void Scene::render() {
	world->render();
}

World* Scene::getWorld() {
	return world;
}
