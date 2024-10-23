#include "ChunkMesh.hpp"
#include "chunk_mesh_builder.hpp"
#include <cstdint>

ChunkMesh::ChunkMesh(Chunk* chunk) : BaseMesh(), chunk(chunk) {
    program = chunk->getWorld()->getEngine()->shaderProgram->getProgram();
	vboFormat = {"1u4"};
	formatSize = calculateFormatSize(vboFormat);
	attrs = {"packed_data"};
	vao = createVAO();
}

std::vector<uint32_t> ChunkMesh::getVertexData() {
    return build_chunk_mesh(chunk->getVoxels(), formatSize, chunk->getPosition(), chunk->getWorld()->getVoxels());
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
		totalSize += std::stoi(fmt.substr(0, 1));  // Get the integer part (3u1 -> 3)
	}
	return totalSize;
}

GLuint ChunkMesh::createVAO() {
	// Create and bind the VAO
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// Get the vertex data for the chunk
	std::vector<uint32_t> vertexData = getVertexData();
	vertexCount = vertexData.size() / formatSize;

	// Create and bind the VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(uint32_t), vertexData.data(), GL_STATIC_DRAW);

	GLint bufferSize = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
	if (bufferSize == 0) {
		return 0; //to optimize?
	}

	// Attribute 0: Position (1 uint32)
	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, formatSize * sizeof(uint32_t), (void*)0);

	/*
	// Attribute 1: Voxel ID (1 uint8)
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, formatSize * sizeof(uint8_t), (void*)(3 * sizeof(uint8_t)));

	// Attribute 2: Face ID (1 uint8)
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, formatSize * sizeof(uint8_t), (void*)(4 * sizeof(uint8_t)));

	// Attribute 3: AO ID (1 uint8)
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, formatSize * sizeof(uint8_t), (void*)(5 * sizeof(uint8_t)));

	// Attribute 4: Flip ID (1 uint8)
	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 1, GL_UNSIGNED_BYTE, formatSize * sizeof(uint8_t), (void*)(6 * sizeof(uint8_t)));
	*/

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
