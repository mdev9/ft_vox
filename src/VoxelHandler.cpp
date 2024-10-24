#include "VoxelHandler.hpp"

VoxelHandler::VoxelHandler(World* world) {
	engine = world->getEngine();
	chunks = world->getChunks();
	interaction_mode = 0;
}

void VoxelHandler::update() {
	rayCast();
}

void VoxelHandler::addVoxel() {
}

void VoxelHandler::removeVoxel() {
	if (voxel_id) {
		chunk->getVoxels()[voxel_index] = 0;
		
		chunk->getMesh()->rebuild();
	}
}

void VoxelHandler::setVoxel() {
	if (interaction_mode)
		addVoxel();
	else
		removeVoxel();
}

void VoxelHandler::switchMode() {
	interaction_mode = !interaction_mode;
}

bool VoxelHandler::rayCast() {
    glm::vec3 start_pos = engine->player->position;
    glm::vec3 end_pos = start_pos + engine->player->forward * MAX_RAY_DIST;

    glm::ivec3 current_voxel_pos = glm::ivec3(start_pos);
    voxel_id = 0;
    voxel_normal = glm::ivec3(0);
    int step_dir = -1;

    float dx = glm::sign(end_pos.x - start_pos.x);
    float dy = glm::sign(end_pos.y - start_pos.y);
    float dz = glm::sign(end_pos.z - start_pos.z);

    float delta_x = (dx != 0) ? glm::min(dx / (end_pos.x - start_pos.x), 10000000.0f) : 10000000.0f;
    float delta_y = (dy != 0) ? glm::min(dy / (end_pos.y - start_pos.y), 10000000.0f) : 10000000.0f;
    float delta_z = (dz != 0) ? glm::min(dz / (end_pos.z - start_pos.z), 10000000.0f) : 10000000.0f;

    float max_x = (dx > 0) ? delta_x * (1.0f - glm::fract(start_pos.x)) : delta_x * glm::fract(start_pos.x);
    float max_y = (dy > 0) ? delta_y * (1.0f - glm::fract(start_pos.y)) : delta_y * glm::fract(start_pos.y);
    float max_z = (dz > 0) ? delta_z * (1.0f - glm::fract(start_pos.z)) : delta_z * glm::fract(start_pos.z);

    while (!(max_x > 1.0f && max_y > 1.0f && max_z > 1.0f)) {
        auto result = get_voxel_id(current_voxel_pos);
        if (std::get<0>(result)) {
            voxel_id = std::get<0>(result);
            voxel_index = std::get<1>(result);
            voxel_local_pos = std::get<2>(result);
            chunk = std::get<3>(result);
            voxel_world_pos = current_voxel_pos;

            if (step_dir == 0) {
                voxel_normal.x = -dx;
            } else if (step_dir == 1) {
                voxel_normal.y = -dy;
            } else {
                voxel_normal.z = -dz;
            }
            return true;
        }

        // Determine next voxel to step into
        if (max_x < max_y) {
            if (max_x < max_z) {
                current_voxel_pos.x += static_cast<int>(dx);
                max_x += delta_x;
                step_dir = 0;
            } else {
                current_voxel_pos.z += static_cast<int>(dz);
                max_z += delta_z;
                step_dir = 2;
            }
        } else {
            if (max_y < max_z) {
                current_voxel_pos.y += static_cast<int>(dy);
                max_y += delta_y;
                step_dir = 1;
            } else {
                current_voxel_pos.z += static_cast<int>(dz);
                max_z += delta_z;
                step_dir = 2;
            }
        }
    }
    return false;
}

std::tuple<int, int, glm::ivec3, Chunk*> VoxelHandler::get_voxel_id(const glm::ivec3& voxel_world_pos) {
    glm::ivec3 chunk_pos = voxel_world_pos / CHUNK_SIZE;
    int cx = chunk_pos.x;
    int cy = chunk_pos.y;
    int cz = chunk_pos.z;

    // Check if the chunk coordinates are within the world bounds
    if (cx >= 0 && cx < WORLD_W && cy >= 0 && cy < WORLD_H && cz >= 0 && cz < WORLD_D) {
        int chunk_index = cx + WORLD_W * cz + WORLD_AREA * cy;

        Chunk* chunk = chunks[chunk_index];

        glm::ivec3 voxel_local_pos = voxel_world_pos - chunk_pos * CHUNK_SIZE;
        int lx = voxel_local_pos.x;
        int ly = voxel_local_pos.y;
        int lz = voxel_local_pos.z;

        int voxel_index = lx + CHUNK_SIZE * lz + CHUNK_AREA * ly;

        int voxel_id = chunk->getVoxels()[voxel_index];

        return {voxel_id, voxel_index, voxel_local_pos, chunk};
    }

    // Return zeros if the voxel is out of bounds
    return {0, 0, glm::ivec3(0), nullptr};
}
