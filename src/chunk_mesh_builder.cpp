#include "chunk_mesh_builder.hpp"
#include "Settings.hpp"

uint32_t pack_data(int x, int y, int z, int voxel_id, int face_id, int ao_id, bool flip_id) {
    // x: 6 bits, y: 6 bits, z: 6 bits
    // voxel_id: 8 bits, face_id: 3 bits, ao_id: 2 bits, flip_id: 1 bit

    uint8_t b_bit = 6, c_bit = 6, d_bit = 8, e_bit = 3, f_bit = 2, g_bit = 1;
    
    uint8_t fg_bit = f_bit + g_bit;        // 2 + 1 = 3
    uint8_t efg_bit = e_bit + fg_bit;      // 3 + 3 = 6
    uint8_t defg_bit = d_bit + efg_bit;    // 8 + 6 = 14
    uint8_t cdefg_bit = c_bit + defg_bit;  // 6 + 14 = 20
    uint8_t bcdefg_bit = b_bit + cdefg_bit; // 6 + 20 = 26

    uint32_t packed_data = 0;

    packed_data |= ((uint32_t)x << bcdefg_bit);
    packed_data |= ((uint32_t)y << cdefg_bit);
    packed_data |= ((uint32_t)z << defg_bit);
    packed_data |= ((uint32_t)voxel_id << efg_bit);
    packed_data |= ((uint32_t)face_id << fg_bit);
    packed_data |= ((uint32_t)ao_id << g_bit);
    packed_data |= (flip_id ? 1 : 0); // Flip_id is a bool, pack it as 1 or 0

    return packed_data;
}

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
		if (local_voxel_pos.x < 0) {
			neighbor_world_voxel_pos.x -= 1;
			neighbor_local_voxel_pos.x = CHUNK_SIZE - 1;
		} else if (local_voxel_pos.x >= CHUNK_SIZE) {
			neighbor_world_voxel_pos.x += 1;
			neighbor_local_voxel_pos.x = 0;
		}

		if (local_voxel_pos.y < 0) {
			neighbor_world_voxel_pos.y -= 1;
			neighbor_local_voxel_pos.y = CHUNK_SIZE - 1;
		} else if (local_voxel_pos.y >= CHUNK_SIZE) {
			neighbor_world_voxel_pos.y += 1;
			neighbor_local_voxel_pos.y = 0;
		}

		if (local_voxel_pos.z < 0) {
			neighbor_world_voxel_pos.z -= 1;
			neighbor_local_voxel_pos.z = CHUNK_SIZE - 1;
		} else if (local_voxel_pos.z >= CHUNK_SIZE) {
			neighbor_world_voxel_pos.z += 1;
			neighbor_local_voxel_pos.z = 0;
		}

		return is_void(neighbor_local_voxel_pos, neighbor_world_voxel_pos, world_voxels);
	}

	int voxel_index = local_voxel_pos.x + local_voxel_pos.z * CHUNK_SIZE + local_voxel_pos.y * CHUNK_AREA;
	return chunk_voxels[voxel_index] == 0; // Return true if it's void
}

int addData(std::vector<uint32_t>& vertexData, int index, std::initializer_list<uint32_t> vertices) {
    for (const auto& vertex : vertices) {
        vertexData[index++] = vertex;
    }
    return index;
}

std::array<int, 4> get_ao(const glm::ivec3& local_pos, const glm::ivec3& world_pos, const std::vector<std::vector<uint8_t>>& world_voxels, char plane) {
    int x = local_pos.x, y = local_pos.y, z = local_pos.z;
    int wx = world_pos.x, wy = world_pos.y, wz = world_pos.z;

    bool a, b, c, d, e, f, g, h;

    if (plane == 'Y') {
        a = is_void({x    , y, z - 1}, {wx    , wy, wz - 1}, world_voxels);
        b = is_void({x - 1, y, z - 1}, {wx - 1, wy, wz - 1}, world_voxels);
        c = is_void({x - 1, y, z    }, {wx - 1, wy, wz    }, world_voxels);
        d = is_void({x - 1, y, z + 1}, {wx - 1, wy, wz + 1}, world_voxels);
        e = is_void({x    , y, z + 1}, {wx    , wy, wz + 1}, world_voxels);
        f = is_void({x + 1, y, z + 1}, {wx + 1, wy, wz + 1}, world_voxels);
        g = is_void({x + 1, y, z    }, {wx + 1, wy, wz    }, world_voxels);
        h = is_void({x + 1, y, z - 1}, {wx + 1, wy, wz - 1}, world_voxels);
    } 
    else if (plane == 'X') {
        a = is_void({x, y    , z - 1}, {wx, wy    , wz - 1}, world_voxels);
        b = is_void({x, y - 1, z - 1}, {wx, wy - 1, wz - 1}, world_voxels);
        c = is_void({x, y - 1, z    }, {wx, wy - 1, wz    }, world_voxels);
        d = is_void({x, y - 1, z + 1}, {wx, wy - 1, wz + 1}, world_voxels);
        e = is_void({x, y    , z + 1}, {wx, wy    , wz + 1}, world_voxels);
        f = is_void({x, y + 1, z + 1}, {wx, wy + 1, wz + 1}, world_voxels);
        g = is_void({x, y + 1, z    }, {wx, wy + 1, wz    }, world_voxels);
        h = is_void({x, y + 1, z - 1}, {wx, wy + 1, wz - 1}, world_voxels);
    } 
    else { // Z plane
        a = is_void({x - 1, y    , z}, {wx - 1, wy    , wz}, world_voxels);
        b = is_void({x - 1, y - 1, z}, {wx - 1, wy - 1, wz}, world_voxels);
        c = is_void({x    , y - 1, z}, {wx    , wy - 1, wz}, world_voxels);
        d = is_void({x + 1, y - 1, z}, {wx + 1, wy - 1, wz}, world_voxels);
        e = is_void({x + 1, y    , z}, {wx + 1, wy    , wz}, world_voxels);
        f = is_void({x + 1, y + 1, z}, {wx + 1, wy + 1, wz}, world_voxels);
        g = is_void({x    , y + 1, z}, {wx    , wy + 1, wz}, world_voxels);
        h = is_void({x - 1, y + 1, z}, {wx - 1, wy + 1, wz}, world_voxels);
    }

    // Calculate the ambient occlusion
    std::array<int, 4> ao = {
        (a + b + c),
        (g + h + a),
        (e + f + g),
        (c + d + e)
    };

    return ao;
}

std::vector<uint32_t> build_chunk_mesh(const std::vector<uint8_t>& chunk_voxels, int format_size, glm::ivec3 chunk_pos, const std::vector<std::vector<uint8_t>>& world_voxels) {
    std::vector<uint32_t> vertexData(CHUNK_VOL * 18 * format_size);
    int index = 0;

    std::array<int, 4> ao;
    bool flip_id;

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                int voxelID = chunk_voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y];
                if (voxelID == 0) continue; // Skip if the voxel is void

				int cx = chunk_pos.x, cy = chunk_pos.y, cz = chunk_pos.z;
				int wx = x + cx * CHUNK_SIZE, wy = y + cy * CHUNK_SIZE, wz = z + cz * CHUNK_SIZE;

                // Check the faces
                // Top face
                if (is_void({x, y + 1, z}, {wx, wy + 1, wz}, world_voxels)) {
					ao = get_ao({x, y + 1, z}, {wx, wy + 1, wz}, world_voxels, 'Y');
					flip_id = ao[1] + ao[3] > ao[0] + ao[2];

                    uint32_t v0 = pack_data(x    , y + 1, z    , voxelID, 0, ao[0], flip_id);
                    uint32_t v1 = pack_data(x + 1, y + 1, z    , voxelID, 0, ao[1], flip_id);
                    uint32_t v2 = pack_data(x + 1, y + 1, z + 1, voxelID, 0, ao[2], flip_id);
                    uint32_t v3 = pack_data(x    , y + 1, z + 1, voxelID, 0, ao[3], flip_id);

					if (flip_id)
						index = addData(vertexData, index, {v1, v0, v3, v1, v3, v2});
					else
						index = addData(vertexData, index, {v0, v3, v2, v0, v2, v1});
                }

                // Bottom face
                if (is_void({x, y - 1, z}, {wx, wy - 1, wz}, world_voxels)) {
					ao = get_ao({x, y - 1, z}, {wx, wy - 1, wz}, world_voxels, 'Y');
					flip_id = ao[1] + ao[3] > ao[0] + ao[2];

                    uint32_t v0 = pack_data(x    , y, z    , voxelID, 1, ao[0], flip_id);
                    uint32_t v1 = pack_data(x + 1, y, z    , voxelID, 1, ao[1], flip_id);
                    uint32_t v2 = pack_data(x + 1, y, z + 1, voxelID, 1, ao[2], flip_id);
                    uint32_t v3 = pack_data(x    , y, z + 1, voxelID, 1, ao[3], flip_id);

					if (flip_id)
						index = addData(vertexData, index, {v1, v3, v0, v1, v2, v3});
					else
						index = addData(vertexData, index, {v0, v2, v3, v0, v1, v2});
                }

                // Right face
                if (is_void({x + 1, y, z}, {wx + 1, wy, wz}, world_voxels)) {
					ao = get_ao({x + 1, y, z}, {wx + 1, wy, wz}, world_voxels, 'X');
					flip_id = ao[1] + ao[3] > ao[0] + ao[2];

                    uint32_t v0 = pack_data(x + 1, y    , z    , voxelID, 2, ao[0], flip_id);
                    uint32_t v1 = pack_data(x + 1, y + 1, z    , voxelID, 2, ao[1], flip_id);
                    uint32_t v2 = pack_data(x + 1, y + 1, z + 1, voxelID, 2, ao[2], flip_id);
                    uint32_t v3 = pack_data(x + 1, y    , z + 1, voxelID, 2, ao[3], flip_id);

					if (flip_id)
						index = addData(vertexData, index, {v3, v0, v1, v3, v1, v2});
					else
						index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                }

                // Left face
                if (is_void({x - 1, y, z}, {wx - 1, wy, wz}, world_voxels)) {
					ao = get_ao({x - 1, y, z}, {wx - 1, wy, wz}, world_voxels, 'X');
					flip_id = ao[1] + ao[3] > ao[0] + ao[2];

                    uint32_t v0 = pack_data(x, y    , z    , voxelID, 3, ao[0], flip_id);
                    uint32_t v1 = pack_data(x, y + 1, z    , voxelID, 3, ao[1], flip_id);
                    uint32_t v2 = pack_data(x, y + 1, z + 1, voxelID, 3, ao[2], flip_id);
                    uint32_t v3 = pack_data(x, y    , z + 1, voxelID, 3, ao[3], flip_id);

					if (flip_id)
						index = addData(vertexData, index, {v3, v1, v0, v3, v2, v1});
					else
						index = addData(vertexData, index, {v0, v2, v1, v0, v3, v2});
                }

                // Back face
                if (is_void({x, y, z - 1}, {wx, wy, wz - 1}, world_voxels)) {
					ao = get_ao({x, y, z -1}, {wx, wy, wz - 1}, world_voxels, 'Z');
					flip_id = ao[1] + ao[3] > ao[0] + ao[2];

                    uint32_t v0 = pack_data(x    , y    , z, voxelID, 4, ao[0], flip_id);
                    uint32_t v1 = pack_data(x    , y + 1, z, voxelID, 4, ao[1], flip_id);
                    uint32_t v2 = pack_data(x + 1, y + 1, z, voxelID, 4, ao[2], flip_id);
                    uint32_t v3 = pack_data(x + 1, y    , z, voxelID, 4, ao[3], flip_id);

					if (flip_id)
						index = addData(vertexData, index, {v3, v0, v1, v3, v1, v2});
					else
						index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                }

                // Front face
                if (is_void({x, y, z + 1}, {wx, wy, wz + 1}, world_voxels)) {
					ao = get_ao({x, y, z + 1}, {wx, wy, wz + 1}, world_voxels, 'Z');
					flip_id = ao[1] + ao[3] > ao[0] + ao[2];

                    uint32_t v0 = pack_data(x    , y    , z + 1, voxelID, 5, ao[0], flip_id);
                    uint32_t v1 = pack_data(x    , y + 1, z + 1, voxelID, 5, ao[1], flip_id);
                    uint32_t v2 = pack_data(x + 1, y + 1, z + 1, voxelID, 5, ao[2], flip_id);
                    uint32_t v3 = pack_data(x + 1, y    , z + 1, voxelID, 5, ao[3], flip_id);

					if (flip_id)
						index = addData(vertexData, index, {v3, v1, v0, v3, v2, v1});
					else
						index = addData(vertexData, index, {v0, v2, v1, v0, v3, v2});
                }
            }
        }
    }

    vertexData.resize(index);
    return vertexData;
}
