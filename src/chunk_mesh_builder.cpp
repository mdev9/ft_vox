#include "chunk_mesh_builder.hpp"
#include <iostream>
#include "Settings.hpp"

bool is_void(const glm::ivec3& voxel_pos, const std::vector<uint8_t>& chunk_voxels) {
    int x = voxel_pos.x;
    int y = voxel_pos.y;
    int z = voxel_pos.z;

    // Out-of-bounds positions are considered void
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE) {
        return true;
    }

    // Check the voxel data at the given position
    int voxelID = chunk_voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y];
    
    return voxelID == 0;
}

int addData(std::vector<uint8_t>& vertexData, int index, std::initializer_list<std::vector<int>> vertices) {
    for (const auto& vertex : vertices) {
        for (const auto& attr : vertex) {
            vertexData[index++] = static_cast<uint8_t>(attr);
        }
    }
    return index;
}

std::vector<uint8_t> build_chunk_mesh(const std::vector<uint8_t>& chunk_voxels, int format_size) {
    std::vector<uint8_t> vertexData(CHUNK_VOL * 18 * format_size);
    int index = 0;

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                int voxelID = chunk_voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y];
                if (!voxelID) continue;

                // Top face
                if (is_void({x, y + 1, z}, chunk_voxels)) {
                    std::vector<int> v0 = {x, y + 1, z, voxelID, 0};
                    std::vector<int> v1 = {x + 1, y + 1, z, voxelID, 0};
                    std::vector<int> v2 = {x + 1, y + 1, z + 1, voxelID, 0};
                    std::vector<int> v3 = {x, y + 1, z + 1, voxelID, 0};

                    index = addData(vertexData, index, {v0, v3, v2, v0, v2, v1});
                }

                // Bottom face
                if (is_void({x, y - 1, z}, chunk_voxels)) {
                    std::vector<int> v0 = {x, y, z, voxelID, 1};
                    std::vector<int> v1 = {x + 1, y, z, voxelID, 1};
                    std::vector<int> v2 = {x + 1, y, z + 1, voxelID, 1};
                    std::vector<int> v3 = {x, y, z + 1, voxelID, 1};

                    index = addData(vertexData, index, {v0, v2, v3, v0, v1, v2});
                }

                // Right face
                if (is_void({x + 1, y, z}, chunk_voxels)) {
                    std::vector<int> v0 = {x + 1, y, z, voxelID, 2};
                    std::vector<int> v1 = {x + 1, y + 1, z, voxelID, 2};
                    std::vector<int> v2 = {x + 1, y + 1, z + 1, voxelID, 2};
                    std::vector<int> v3 = {x + 1, y, z + 1, voxelID, 2};

                    index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                }

                // Left face
                if (is_void({x - 1, y, z}, chunk_voxels)) {
                    std::vector<int> v0 = {x, y, z, voxelID, 3};
                    std::vector<int> v1 = {x, y + 1, z, voxelID, 3};
                    std::vector<int> v2 = {x, y + 1, z + 1, voxelID, 3};
                    std::vector<int> v3 = {x, y, z + 1, voxelID, 3};

                    index = addData(vertexData, index, {v0, v2, v1, v0, v3, v2});
                }

                // Back face
                if (is_void({x, y, z - 1}, chunk_voxels)) {
                    std::vector<int> v0 = {x, y, z, voxelID, 4};
                    std::vector<int> v1 = {x, y + 1, z, voxelID, 4};
                    std::vector<int> v2 = {x + 1, y + 1, z, voxelID, 4};
                    std::vector<int> v3 = {x + 1, y, z, voxelID, 4};

                    index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                }

                // Front face
                if (is_void({x, y, z + 1}, chunk_voxels)) {
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
