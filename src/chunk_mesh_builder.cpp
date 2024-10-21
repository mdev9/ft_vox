#include "chunk_mesh_builder.hpp"
#include <cstdint>
#include <iostream>
#include "Settings.hpp"

int get_chunk_index(const glm::ivec3& world_voxel_pos) {
	int wx = world_voxel_pos.x;
	int wy = world_voxel_pos.y;
	int wz = world_voxel_pos.z;

	// Adjust for negative coordinates by shifting them to positive space
	int cx = (wx >= 0) ? wx / CHUNK_SIZE : (wx - (CHUNK_SIZE - 1)) / CHUNK_SIZE;
	int cy = (wy >= 0) ? wy / CHUNK_SIZE : (wy - (CHUNK_SIZE - 1)) / CHUNK_SIZE;
	int cz = (wz >= 0) ? wz / CHUNK_SIZE : (wz - (CHUNK_SIZE - 1)) / CHUNK_SIZE;

	// Ensure the chunk coordinates are within world bounds
	if (cx < 0 || cx >= WORLD_W || cy < 0 || cy >= WORLD_H || cz < 0 || cz >= WORLD_D) {
		return -1;
	}

	return cx + WORLD_W * cz + WORLD_AREA * cy;
}

bool is_void(const glm::ivec3& local_voxel_pos, const glm::ivec3& world_voxel_pos, const std::vector<std::vector<uint8_t>>& world_voxels) {
    int chunk_index = get_chunk_index(world_voxel_pos);
    
    if (chunk_index < 0 || (size_t)chunk_index >= world_voxels.size()) {
        if (chunk_index != -1)
            std::cerr << "Invalid chunk_index too big: " << chunk_index << std::endl;
        return true; // Treat as solid if out of bounds
    }

    const std::vector<uint8_t>& chunk_voxels = world_voxels[chunk_index];
    if (chunk_voxels.size() != CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) { // this avoids segfaulting !IMPORTANT!
        return false; // Handle invalid chunk voxel data
    }

	if (local_voxel_pos.x < 0 || local_voxel_pos.x >= CHUNK_SIZE ||
			local_voxel_pos.y < 0 || local_voxel_pos.y >= CHUNK_SIZE ||
			local_voxel_pos.z < 0 || local_voxel_pos.z >= CHUNK_SIZE) {

		// Adjacent chunk check
		glm::ivec3 neighbor_world_voxel_pos = world_voxel_pos;
		glm::ivec3 neighbor_local_voxel_pos = local_voxel_pos;

		// Adjust the positions for adjacent chunks based on which face we're checking
		if (local_voxel_pos.x < 0) { // Left neighbor
			neighbor_world_voxel_pos.x -= 1;
			neighbor_local_voxel_pos.x = CHUNK_SIZE - 1; // Wrap to opposite side of the chunk
		} else if (local_voxel_pos.x >= CHUNK_SIZE) { // Right neighbor
			neighbor_world_voxel_pos.x += 1;
			neighbor_local_voxel_pos.x = 0;
		}

		if (local_voxel_pos.y < 0) { // Below neighbor
			neighbor_world_voxel_pos.y -= 1;
			neighbor_local_voxel_pos.y = CHUNK_SIZE - 1;
		} else if (local_voxel_pos.y >= CHUNK_SIZE) { // Above neighbor
			neighbor_world_voxel_pos.y += 1;
			neighbor_local_voxel_pos.y = 0;
		}

		if (local_voxel_pos.z < 0) { // Back neighbor
			neighbor_world_voxel_pos.z -= 1;
			neighbor_local_voxel_pos.z = CHUNK_SIZE - 1;
		} else if (local_voxel_pos.z >= CHUNK_SIZE) { // Front neighbor
			neighbor_world_voxel_pos.z += 1;
			neighbor_local_voxel_pos.z = 0;
		}

		// Call is_void recursively for the adjacent chunk
		return is_void(neighbor_local_voxel_pos, neighbor_world_voxel_pos, world_voxels);
	}

	// Calculate the voxel index within the chunk
	int voxel_index = local_voxel_pos.x + local_voxel_pos.z * CHUNK_SIZE + local_voxel_pos.y * CHUNK_AREA;
	return chunk_voxels[voxel_index] == 0; // Return true if it's void
}

int addData(std::vector<uint8_t>& vertexData, int index, std::initializer_list<std::vector<int>> vertices) {
	for (const auto& vertex : vertices) {
		for (const auto& attr : vertex) {
			vertexData[index++] = static_cast<uint8_t>(attr);
		}
	}
	return index;
}

std::vector<uint8_t> build_chunk_mesh(const std::vector<uint8_t>& chunk_voxels, int format_size, glm::ivec3 chunk_pos, const std::vector<std::vector<uint8_t>>& world_voxels) {
    std::vector<uint8_t> vertexData(CHUNK_VOL * 18 * format_size);
    int index = 0;

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                int voxelID = chunk_voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y];
                if (voxelID == 0) continue; // Skip if the voxel is void

                // Check the faces
                // Top face
                if (is_void({x, y + 1, z}, chunk_pos * CHUNK_SIZE + glm::ivec3(x, y + 1, z), world_voxels)) {
                    std::vector<int> v0 = {x, y + 1, z, voxelID, 0};
                    std::vector<int> v1 = {x + 1, y + 1, z, voxelID, 0};
                    std::vector<int> v2 = {x + 1, y + 1, z + 1, voxelID, 0};
                    std::vector<int> v3 = {x, y + 1, z + 1, voxelID, 0};

                    index = addData(vertexData, index, {v0, v3, v2, v0, v2, v1});
                }

                // Bottom face
                if (is_void({x, y - 1, z}, chunk_pos * CHUNK_SIZE + glm::ivec3(x, y - 1, z), world_voxels)) {
                    std::vector<int> v0 = {x, y, z, voxelID, 1};
                    std::vector<int> v1 = {x + 1, y, z, voxelID, 1};
                    std::vector<int> v2 = {x + 1, y, z + 1, voxelID, 1};
                    std::vector<int> v3 = {x, y, z + 1, voxelID, 1};

                    index = addData(vertexData, index, {v0, v2, v3, v0, v1, v2});
                }

                // Right face
                if (is_void({x + 1, y, z}, chunk_pos * CHUNK_SIZE + glm::ivec3(x + 1, y, z), world_voxels)) {
                    std::vector<int> v0 = {x + 1, y, z, voxelID, 2};
                    std::vector<int> v1 = {x + 1, y + 1, z, voxelID, 2};
                    std::vector<int> v2 = {x + 1, y + 1, z + 1, voxelID, 2};
                    std::vector<int> v3 = {x + 1, y, z + 1, voxelID, 2};

                    index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                }

                // Left face
                if (is_void({x - 1, y, z}, chunk_pos * CHUNK_SIZE + glm::ivec3(x - 1, y, z), world_voxels)) {
                    std::vector<int> v0 = {x, y, z, voxelID, 3};
                    std::vector<int> v1 = {x, y + 1, z, voxelID, 3};
                    std::vector<int> v2 = {x, y + 1, z + 1, voxelID, 3};
                    std::vector<int> v3 = {x, y, z + 1, voxelID, 3};

                    index = addData(vertexData, index, {v0, v2, v1, v0, v3, v2});
                }

                // Back face
                if (is_void({x, y, z - 1}, chunk_pos * CHUNK_SIZE + glm::ivec3(x, y, z - 1), world_voxels)) {
                    std::vector<int> v0 = {x, y, z, voxelID, 4};
                    std::vector<int> v1 = {x, y + 1, z, voxelID, 4};
                    std::vector<int> v2 = {x + 1, y + 1, z, voxelID, 4};
                    std::vector<int> v3 = {x + 1, y, z, voxelID, 4};

                    index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                }

                // Front face
                if (is_void({x, y, z + 1}, chunk_pos * CHUNK_SIZE + glm::ivec3(x, y, z + 1), world_voxels)) {
                    std::vector<int> v0 = {x, y, z + 1, voxelID, 5};
                    std::vector<int> v1 = {x, y + 1, z + 1, voxelID, 5};
                    std::vector<int> v2 = {x + 1, y + 1, z + 1, voxelID, 5};
                    std::vector<int> v3 = {x + 1, y, z + 1, voxelID, 5};

                    index = addData(vertexData, index, {v0, v2, v1, v0, v3, v2});
                }
            }
        }
    }

    vertexData.resize(index);
    return vertexData;
}
