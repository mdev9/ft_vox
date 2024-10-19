#include "QuadMesh.hpp"

QuadMesh::QuadMesh(ShaderProgram* shaderProgram) {
    this->program = shaderProgram->getProgram();
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    createMesh();
}

void QuadMesh::createMesh() {
    auto vertexData = getVertexData();
    vertexCount = vertexData.size() / 6;

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    
    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Vertex colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

std::vector<float> QuadMesh::getVertexData() {
    std::vector<float> vertexData = {
        // Position            // Color
         0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  // Top Right
        -0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,  // Top Left
        -0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f,  // Bottom Left
         0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  // Top Right
        -0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f,  // Bottom Left
         0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f   // Bottom Right
    };

    return vertexData;
}
