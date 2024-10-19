#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "Settings.hpp"
#include "ChunkMesh.hpp"
#include "VoxelEngine.hpp"
#include <vector>

class ChunkMesh;

class VoxelEngine;

class Chunk {
public:
    Chunk(VoxelEngine* engine);
	~Chunk();

    void buildMesh();
    void render();
    std::vector<uint8_t> buildVoxels();

    const std::vector<uint8_t>& getVoxels() const { return voxels; }
	VoxelEngine* getEngine();

private:
    VoxelEngine* engine;
    std::vector<uint8_t> voxels;
    ChunkMesh* mesh;
};

#endif
