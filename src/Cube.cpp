#include "Cube.hpp"

Cube::Cube(GLuint texture) : texture(texture) { // Initialize texture
    setupCube();
}

void Cube::setupCube() {
    float vertices[] = {
        // positions          // texture coords
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
    };

    unsigned int indices[] = {
        // front face
        0, 1, 2, 2, 3, 0,
        // back face
        4, 5, 6, 6, 7, 4,
        // left face
        0, 3, 7, 7, 4, 0,
        // right face
        1, 2, 6, 6, 5, 1,
        // bottom face
        0, 4, 5, 5, 1, 0,
        // top face
        2, 3, 7, 7, 6, 2
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // unbind VAO
}

void Cube::render(unsigned int shaderID) {
    // Use the shader program
    glUseProgram(shaderID); 

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, texture); 
    glBindVertexArray(VAO);
    
    // Draw the cube
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0); // unbind VAO
}
