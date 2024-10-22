#ifndef CHUNK_MESH_BUILDER_HPP
#define CHUNK_MESH_BUILDER_HPP

#include <vector>
#include "Settings.hpp"

std::vector<uint8_t> build_chunk_mesh(const std::vector<uint8_t>& chunk_voxels, int format_size, glm::ivec3 chunk_position, const std::vector<std::vector<uint8_t>>& world_voxels);

#endif
