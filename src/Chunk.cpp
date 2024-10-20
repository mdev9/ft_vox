#include "Chunk.hpp"
#include "VoxelEngine.hpp"
#include <glm/gtc/noise.hpp>

Chunk::Chunk(VoxelEngine* engine) {
	this->engine = engine;
    voxels = buildVoxels();
    buildMesh();
}

void Chunk::buildMesh() {
    mesh = new ChunkMesh(this);
}

void Chunk::render() {
	mesh->render();
}

std::vector<uint8_t> Chunk::buildVoxels() {
    std::vector<uint8_t> voxels(CHUNK_VOL, 0);

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            for (int y = 0; y < CHUNK_SIZE; ++y) {
                // Get the simplex noise value for the given coordinates
                float noise = glm::simplex(glm::vec3(x, y, z) * 0.1f);

                // Check if the noise value, when added to 1, is greater than 0 (i.e., positive)
                if ((int)(noise + 1) > 0) {
                    // Fill voxel with x + y + z if the noise condition is met
                    voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] = x + y + z;
                }
            }
        }
    }

    return voxels;
}

VoxelEngine* Chunk::getEngine() {
	return engine;
}

Chunk::~Chunk() {
	delete mesh;
}
