#ifndef CHUNK_MESH_BUILDER_HPP
#define CHUNK_MESH_BUILDER_HPP

#include <vector>
#include "Settings.hpp"

bool is_void(const glm::ivec3& voxel_pos, const std::vector<uint8_t>& chunk_voxels);
int add_data(std::vector<uint8_t>& vertex_data, int index, const std::vector<int>& vertex);
std::vector<uint8_t> build_chunk_mesh(const std::vector<uint8_t>& chunk_voxels, int format_size, glm::ivec3 chunk_pos, std::vector<uint8_t> world_voxels);

#endif
