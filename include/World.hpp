#ifndef WORLD_HPP
#define WORLD_HPP

#include "Settings.hpp"
#include "Chunk.hpp"
#include "VoxelEngine.hpp"
#include "VoxelHandler.hpp"

class VoxelHandler;

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
		VoxelHandler* getVoxelHandler();
		std::vector<Chunk*> getChunks();
		std::vector<std::vector<uint8_t>> getVoxels();

	private:
		VoxelEngine* engine;
		VoxelHandler* voxelHandler;
		std::vector<Chunk*> chunks;
		std::vector<std::vector<uint8_t>> voxels;
};

#endif
