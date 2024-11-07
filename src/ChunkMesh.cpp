#include "ChunkMesh.hpp"
#include "VoxelHandler.hpp"
#include "chunk_mesh_builder.hpp"
#include <cstdint>

ChunkMesh::ChunkMesh(Chunk* chunk) : BaseMesh(), chunk(chunk) {
    program = chunk->getWorld()->getEngine()->shaderProgram->getProgram();
	vboFormat = {"1u4"};
	formatSize = calculateFormatSize(vboFormat);
	attrs = {"packed_data"};
	vao = createVAO(0);
}

void ChunkMesh::rebuild(VoxelHandler *handler) {
	if (vao) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
	if (vbo) {
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}

	// Generate new VAO
	vao = createVAO(handler);
}

std::vector<uint32_t> ChunkMesh::getVertexData(VoxelHandler *handler) {
    return build_chunk_mesh(chunk->getVoxels(), formatSize, chunk->getPosition(), chunk->getWorld()->getVoxels(), handler);
}

void ChunkMesh::render() {
    glUseProgram(program);
    glBindVertexArray(vao);  
    if (vertexCount > 0) {
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    } else {
        std::cerr << "No vertices to render!" << std::endl;
    }
    glBindVertexArray(0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}

int ChunkMesh::calculateFormatSize(const std::vector<std::string>& format) {
	int totalSize = 0;
	for (const auto& fmt : format) {
		totalSize += std::stoi(fmt.substr(0, 1));
	}
	return totalSize;
}

GLuint ChunkMesh::createVAO(VoxelHandler *handler) {
	// Create and bind the VAO
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// Get the vertex data for the chunk
	std::vector<uint32_t> vertexData = getVertexData(handler);
	vertexCount = vertexData.size() / formatSize;

	// Create and bind the VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(uint32_t), vertexData.data(), GL_STATIC_DRAW);

	GLint bufferSize = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
	if (bufferSize == 0) {
		return 0;
	}

	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, formatSize * sizeof(uint32_t), (void*)0);

	// Unbind the VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error: " << err << std::endl;
	}

	return vaoID;
}

ChunkMesh::~ChunkMesh() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
