#ifndef VOXELHANDLER_HPP
#define VOXELHANDLER_HPP

#include "Chunk.hpp"
#include "VoxelEngine.hpp"

class Chunk;

class VoxelHandler {
	public:
		VoxelHandler(World* world);

		void update();
		bool rayCast();
		void addVoxel();
		void removeVoxel();
		void setVoxel();
		void switchMode();
		std::tuple<int, int, glm::ivec3, Chunk*> get_voxel_id(const glm::ivec3& voxel_world_pos);

		VoxelEngine *engine;
		std::vector<Chunk *> chunks;

	private:
		Chunk *chunk;
		int voxel_id;
		int voxel_index;
		glm::ivec3 voxel_local_pos;
		glm::ivec3 voxel_world_pos;
		glm::ivec3 voxel_normal;
		bool interaction_mode;
};

#endif
