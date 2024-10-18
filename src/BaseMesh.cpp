#include "BaseMesh.hpp"

BaseMesh::BaseMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
}

BaseMesh::~BaseMesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

GLuint BaseMesh::getVAO() {
    return vao;
}

GLuint BaseMesh::getProgram() {
    return program;
}

void BaseMesh::setupMesh() {
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
