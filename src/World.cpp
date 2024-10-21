#include "World.hpp"
#include "VoxelEngine.hpp"

World::World(VoxelEngine* engine) : engine(engine) {
    chunks.resize(WORLD_VOL, nullptr);
    voxels.resize(WORLD_VOL * CHUNK_VOL);

	std::cout << "World Size: " << WORLD_W << "*" << WORLD_H << "*" << WORLD_D << " chunks\n";

	std::cout << "Building chunks ..." << std::flush;
    auto startChunks = std::chrono::high_resolution_clock::now();
    buildChunks();
    auto endChunks = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedChunks = endChunks - startChunks;
    std::cout << "\tDone in " << elapsedChunks.count() << "s!" << std::endl;

	std::cout << "Building mesh ..." << std::flush;
    auto startMesh = std::chrono::high_resolution_clock::now();
    buildChunkMesh();
    auto endMesh = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedMesh = endMesh - startMesh;
    std::cout << "\tDone in " << elapsedMesh.count() << "s!" << std::endl;
}

void World::buildChunks() {
	voxels.resize(WORLD_W * WORLD_H * WORLD_D);

    for (int x = 0; x < WORLD_W; ++x) {
        for (int y = 0; y < WORLD_H; ++y) {
            for (int z = 0; z < WORLD_D; ++z) {
                glm::ivec3 position(x, y, z);
                Chunk* chunk = new Chunk(this, position);
                int chunk_index = x + WORLD_W * z + WORLD_AREA * y;
                this->chunks[chunk_index] = chunk;

                // Build the chunk's voxels
                std::vector<uint8_t> chunkVoxels = chunk->getVoxels();

                // Store chunk voxels in world_voxels
                voxels[chunk_index] = chunkVoxels;
            }
        }
    }
}

void World::buildChunkMesh() {
    for (Chunk* chunk : chunks) {
        chunk->buildMesh();
    }
}

void World::update() {
    // Any world updates
}

void World::render() {
    for (Chunk* chunk : chunks) {
        chunk->render();
    }
}

VoxelEngine* World::getEngine() {
	return engine;
}

std::vector<std::vector<uint8_t>> World::getVoxels() {
	return voxels;
}

World::~World() {
    for (auto chunk : chunks) {
        delete chunk;
    }
}
