#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "Chunk.hpp"
#include "VoxelEngine.hpp"

class Chunk;

class VoxelEngine;

class Scene {
	public:
		Scene(VoxelEngine* engine);

		void update();
		void render();

	private:
		Chunk* chunk;
};

#endif
