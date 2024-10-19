#include "Chunk.hpp"
#include "VoxelEngine.hpp"

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
                voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] = x + y + z;
                //voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] = 1;
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
