#include "QuadMesh.hpp"

QuadMesh::QuadMesh(ShaderProgram* shaderProgram) {
    this->program = shaderProgram->getProgram();
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    createMesh();
}

void QuadMesh::createMesh() {
    auto vertexData = getVertexData();
    vertexCount = vertexData.size();

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), vertexData.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (GLvoid*)(sizeof(glm::vec3))); // Color
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

std::vector<glm::vec3> QuadMesh::getVertexData() {
    return {
        {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f}, {-0.5f, -0.5f, 0.0f},
        {0.5f, 0.5f, 0.0f}, {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}
    };
}
