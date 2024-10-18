#include "QuadMesh.hpp"

QuadMesh::QuadMesh() {
    initializeMesh();
}

void QuadMesh::initializeMesh() {
    vertices = {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f }
    };

    uvs = {
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f }
    };

    normals = {
        { 0.0f, 0.0f, 1.0f }, // All normals point forward
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f }
    };

    setupMesh(); // Setup buffers using the base class method
}

void QuadMesh::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // Drawing the quad
    glBindVertexArray(0);
}
