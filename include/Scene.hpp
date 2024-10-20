#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "World.hpp"
#include "VoxelEngine.hpp"

class World;

class VoxelEngine;

class Scene {
	public:
		Scene(VoxelEngine* engine);

		void update();
		void render();

	private:
		World* world;
};

#endif
