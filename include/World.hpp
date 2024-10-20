#ifndef WORLD_HPP
#define WORLD_HPP

#include "Settings.hpp"
#include "Chunk.hpp"
#include "VoxelEngine.hpp"

class VoxelEngine;

class Chunk;

class World {
	public:
		World(VoxelEngine* engine);
		~World();

		void buildChunks();
		void buildChunkMesh();
		void update();
		void render();

		VoxelEngine* getEngine();
		std::vector<uint8_t> getVoxels();

	private:
		VoxelEngine* engine;
		std::vector<Chunk*> chunks;
		std::vector<uint8_t> voxels;
};

#endif
