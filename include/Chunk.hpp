#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "Settings.hpp"
#include "ChunkMesh.hpp"
#include "World.hpp"
#include <vector>

class World;

class ChunkMesh;

class Chunk {
	public:
		Chunk(World* world, const glm::ivec3& position);
		~Chunk();

		void buildMesh();
		void render();
		void setUniform();

		World* getWorld();
		std::vector<uint8_t>& getVoxels();
		glm::ivec3 getPosition();

	private:
		World* world;
		glm::ivec3 position;
		glm::mat4 m_model;
		std::vector<uint8_t> voxels;
		ChunkMesh* mesh;
		bool is_empty;

		glm::mat4 getModelMatrix();
		std::vector<uint8_t> buildVoxels();
};

#endif
