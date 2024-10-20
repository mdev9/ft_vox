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
    for (int x = 0; x < WORLD_W; ++x) {
        for (int y = 0; y < WORLD_H; ++y) {
            for (int z = 0; z < WORLD_D; ++z) {
                // Declare position using glm::ivec3
                glm::ivec3 position(x, y, z); // Correctly initialize position

                // Create the Chunk instance using the initialized position
                Chunk* chunk = new Chunk(this, position);

                int chunk_index = x + WORLD_W * z + WORLD_AREA * y;
                this->chunks[chunk_index] = chunk;

                // Build the chunk's voxels
                std::vector<uint8_t> chunkVoxels = chunk->getVoxels();

                // Insert the chunk voxels into the main voxels vector
                voxels.insert(voxels.end(), chunkVoxels.begin(), chunkVoxels.end());
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

World::~World() {
    for (auto chunk : chunks) {
        delete chunk;
    }
}
