#include "ChunkMesh.hpp"
#include "chunk_mesh_builder.hpp"

ChunkMesh::ChunkMesh(Chunk* chunk) : BaseMesh(), chunk(chunk) {
    program = chunk->getEngine()->shaderProgram->getProgram();
	// 3 floats for position, 1 uint8 for voxel_id, 1 uint8 for face_id
	vboFormat = {"3u1", "1u1", "1u1"};
	formatSize = calculateFormatSize(vboFormat);
	attrs = {"in_position", "voxel_id", "face_id"};
	vao = createVAO();
}

std::vector<uint8_t> ChunkMesh::getVertexData() {
    return build_chunk_mesh(chunk->getVoxels(), formatSize);
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
	std::vector<uint8_t> vertexData = getVertexData();
	vertexCount = vertexData.size() / formatSize;

	std::cout << "Vertex data size: " << vertexData.size() << std::endl;
	std::cout << "Vertex count: " << vertexCount << std::endl;

	// Create and bind the VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size(), vertexData.data(), GL_STATIC_DRAW);

	GLint bufferSize = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
	if (bufferSize == 0) {
		std::cerr << "VBO is empty!" << std::endl;
	}

	// Attribute 0: Position (3 uint8)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE, formatSize * sizeof(uint8_t), (void*)0);

	// Attribute 1: Voxel ID (1 uint8)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, formatSize * sizeof(uint8_t), (void*)(3 * sizeof(uint8_t)));

	// Attribute 2: Face ID (1 uint8)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, formatSize * sizeof(uint8_t), (void*)(4 * sizeof(uint8_t)));

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
