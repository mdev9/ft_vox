#include "Chunk.hpp"
#include "VoxelEngine.hpp"
#include <glm/gtc/noise.hpp>
#include <algorithm>

Chunk::Chunk(World* world, const glm::ivec3& position)
    : world(world), position(position), mesh(nullptr), is_empty(true) {
    m_model = getModelMatrix();
    voxels = buildVoxels();
	buildMesh();
}

Chunk::~Chunk() {
	if (mesh)
		delete mesh;
}

glm::mat4 Chunk::getModelMatrix() {
    return glm::translate(glm::mat4(1.0f), glm::vec3(position) * static_cast<float>(CHUNK_SIZE));
}

void Chunk::buildMesh() {
	if (!is_empty)
		mesh = new ChunkMesh(this);
}

void Chunk::render() {
	if (!is_empty) {
		setUniform();
		mesh->render();
	}
}

void Chunk::setUniform() {
    // Ensure the shader program is active
    glUseProgram(getWorld()->getEngine()->shaderProgram->getProgram());

    // Set the model matrix for this chunk
    GLint mModelLoc = glGetUniformLocation(getWorld()->getEngine()->shaderProgram->getProgram(), "m_model");
    glUniformMatrix4fv(mModelLoc, 1, GL_FALSE, glm::value_ptr(m_model));
}

//todo: multithread this function
std::vector<uint8_t> Chunk::buildVoxels() {
	std::vector<uint8_t> voxels(CHUNK_VOL, 0);
    float scale = 0.01f;

    int cx = position.x * CHUNK_SIZE;
    int cy = position.y * CHUNK_SIZE;
    int cz = position.z * CHUNK_SIZE;

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        int wx = x + cx;
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            int wz = z + cz;
            int worldHeight = static_cast<int>(glm::simplex(glm::vec2(wx, wz) * scale) * 32 + 32);
            int localHeight = std::min(worldHeight - cy, CHUNK_SIZE);

            for (int y = 0; y < localHeight; ++y) {
                int wy = y + cy;
                voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] = static_cast<uint8_t>(wy + 1);
            }
        }
    }

    is_empty = std::all_of(voxels.begin(), voxels.end(), [](uint8_t v) { return v == 0; });
    return voxels;
}

World* Chunk::getWorld() {
	return world;
}

std::vector<uint8_t>& Chunk::getVoxels() {
	return voxels;
}
