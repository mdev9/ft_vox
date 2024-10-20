#include "chunk_mesh_builder.hpp"
#include <cstdint>
#include <iostream>
#include "Settings.hpp"

int get_chunk_index(const glm::ivec3& world_voxel_pos) {
    int wx = world_voxel_pos.x;
    int wy = world_voxel_pos.y;
    int wz = world_voxel_pos.z;

    int cx = wx / CHUNK_SIZE;
    int cy = wy / CHUNK_SIZE;
    int cz = wz / CHUNK_SIZE;

    // Ensure the chunk coordinates are within world bounds
    if (cx < 0 || cx >= WORLD_W || cy < 0 || cy >= WORLD_H || cz < 0 || cz >= WORLD_D) {
        return -1;
    }

    return cx + WORLD_W * cz + WORLD_AREA * cy;
}

bool is_void(const glm::ivec3& local_voxel_pos, const glm::ivec3& world_voxel_pos, const std::vector<uint8_t>& world_voxels) {
    int chunk_index = get_chunk_index(world_voxel_pos);
    if (chunk_index == -1) {
        return false;
    }

    const auto& chunk_voxels = world_voxels;

    int x = local_voxel_pos.x;
    int y = local_voxel_pos.y;
    int z = local_voxel_pos.z;

	int voxel_index = ((x % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE) +
                      ((z % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE) * CHUNK_SIZE +
                      ((y % CHUNK_SIZE + CHUNK_SIZE) % CHUNK_SIZE) * CHUNK_AREA;

    if (voxel_index < 0 || (unsigned int)voxel_index >= chunk_voxels.size()) {
        std::cerr << "Error: voxel index out of bounds: " << voxel_index << std::endl;
        return true;
    }

    return chunk_voxels[voxel_index] == 0;
}

// Function to add vertex data
int addData(std::vector<uint8_t>& vertexData, int index, std::initializer_list<std::vector<int>> vertices) {
    for (const auto& vertex : vertices) {
        if (index + vertex.size() > vertexData.size()) {
            std::cerr << "Error: Out of bounds in vertexData at index " << index << " with vertex size " << vertex.size() << "\n";
            return index;
        }
        
        for (const auto& attr : vertex) {
            vertexData[index++] = static_cast<uint8_t>(attr);
        }
    }
    return index;
}

std::vector<uint8_t> build_chunk_mesh(const std::vector<uint8_t>& chunk_voxels, int format_size, glm::ivec3 chunk_pos, std::vector<uint8_t> world_voxels) {
    std::vector<uint8_t> vertexData(CHUNK_VOL * 36 * format_size);
    int index = 0;

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                int voxelID = chunk_voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y];
                if (!voxelID) continue;

                // World voxel pos
                glm::ivec3 w = {
                    x + chunk_pos.x * CHUNK_SIZE,
                    y + chunk_pos.y * CHUNK_SIZE,
                    z + chunk_pos.z * CHUNK_SIZE
                };

                // Top face
                if (is_void({x, y + 1, z}, {w.x, w.y + 1, w.z}, world_voxels)) {
                    std::vector<int> v0 = {x    , y + 1, z    , voxelID, 0};
                    std::vector<int> v1 = {x + 1, y + 1, z    , voxelID, 0};
                    std::vector<int> v2 = {x + 1, y + 1, z + 1, voxelID, 0};
                    std::vector<int> v3 = {x    , y + 1, z + 1, voxelID, 0};
                    index = addData(vertexData, index, {v0, v3, v2, v0, v2, v1});
                }

                // Bottom face
                if (is_void({x, y - 1, z}, {w.x, w.y - 1, w.z}, world_voxels)) {
                    std::vector<int> v0 = {x    , y, z    , voxelID, 1};
                    std::vector<int> v1 = {x + 1, y, z    , voxelID, 1};
                    std::vector<int> v2 = {x + 1, y, z + 1, voxelID, 1};
                    std::vector<int> v3 = {x    , y, z + 1, voxelID, 1};
                    index = addData(vertexData, index, {v0, v2, v3, v0, v1, v2});
                }

                // Right face
                if (is_void({x + 1, y, z}, {w.x + 1, w.y, w.z}, world_voxels)) {
                    std::vector<int> v0 = {x + 1, y    , z    , voxelID, 2};
                    std::vector<int> v1 = {x + 1, y + 1, z    , voxelID, 2};
                    std::vector<int> v2 = {x + 1, y + 1, z + 1, voxelID, 2};
                    std::vector<int> v3 = {x + 1, y    , z + 1, voxelID, 2};
                    index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                }

                // Left face
                if (is_void({x - 1, y, z}, {w.x - 1, w.y, w.z}, world_voxels)) {
                    std::vector<int> v0 = {x, y    , z    , voxelID, 3};
                    std::vector<int> v1 = {x, y + 1, z    , voxelID, 3};
                    std::vector<int> v2 = {x, y + 1, z + 1, voxelID, 3};
                    std::vector<int> v3 = {x, y    , z + 1, voxelID, 3};
                    index = addData(vertexData, index, {v0, v2, v1, v0, v3, v2});
                }

                // Back face
                if (is_void({x, y, z - 1}, {w.x, w.y, w.z - 1}, world_voxels)) {
                    std::vector<int> v0 = {x    , y    , z, voxelID, 4};
                    std::vector<int> v1 = {x    , y + 1, z, voxelID, 4};
                    std::vector<int> v2 = {x + 1, y + 1, z, voxelID, 4};
                    std::vector<int> v3 = {x + 1, y    , z, voxelID, 4};
                    index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                }

                // Front face
                if (is_void({x, y, z + 1}, {w.x, w.y, w.z + 1}, world_voxels)) {
                    std::vector<int> v0 = {x	, y    , z + 1, voxelID, 5};
                    std::vector<int> v1 = {x	, y + 1, z + 1, voxelID, 5};
                    std::vector<int> v2 = {x + 1, y + 1, z + 1, voxelID, 5};
                    std::vector<int> v3 = {x + 1, y    , z + 1, voxelID, 5};
                    index = addData(vertexData, index, {v0, v2, v1, v0, v3, v2});
                }
            }
        }
    }

    vertexData.resize(index);

    return vertexData;
}
