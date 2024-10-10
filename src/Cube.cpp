#include "Cube.hpp"
#include <glm/gtc/type_ptr.hpp>

Cube::Cube(const Texture& texture)
    : isMultiTexture(false), topTexture(""), bottomTexture(""), texture(texture) {
    setupCube();
}

Cube::Cube(const Texture& topTexture, const Texture& sideTexture, const Texture& bottomTexture)
    : isMultiTexture(true), topTexture(topTexture), bottomTexture(bottomTexture), texture(sideTexture){
    setupCube();
}

void Cube::setupCube() {
	float vertices[] = {
		// Position          // Texture Coords (flipped y-axis for sides)
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  // Bottom-left
		0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  // Bottom-right
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  // Top-left

		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // Bottom-left
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  // Bottom-right
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  // Top-right
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // Top-left

		// Left face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // Bottom-left
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  // Bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Top-right
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // Top-left

		// Right face
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // Bottom-left
		0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  // Bottom-right
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Top-right
		0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // Top-left

		// Top face
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  // Top-left
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // Top-right
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // Bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // Bottom-left

		// Bottom face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // Top-left
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // Top-right
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  // Bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f   // Bottom-left
	};

	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0, // Front
		4, 5, 6, 6, 7, 4, // Back
		8, 9, 10, 10, 11, 8, // Left
		12, 13, 14, 14, 15, 12, // Right
		16, 17, 18, 18, 19, 16, // Top
		20, 21, 22, 22, 23, 20  // Bottom
	};

	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    // Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO (good practice)
    glBindVertexArray(0);
}

void Cube::render(GLuint shaderID) {
    (void) shaderID; // Suppress unused parameter warning

    glBindVertexArray(VAO); // Bind the VAO to render the cube

    if (isMultiTexture) {
        // Bind the top texture and render the top face
        glBindTexture(GL_TEXTURE_2D, topTexture.getID());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(unsigned int))); // Top face: indices 24-29

        // Bind the bottom texture and render the bottom face
        glBindTexture(GL_TEXTURE_2D, bottomTexture.getID());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30 * sizeof(unsigned int))); // Bottom face: indices 30-35

        // Bind the side texture and render the side faces
        glBindTexture(GL_TEXTURE_2D, texture.getID());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));  // Front face: indices 0-5
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));  // Back face: indices 6-11
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int))); // Left face: indices 12-17
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(unsigned int))); // Right face: indices 18-23
    } else {
        // Render as a single texture cube
        glBindTexture(GL_TEXTURE_2D, texture.getID());
        // Render all faces with the single texture
        for (int i = 0; i < 6; ++i) {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int))); // Adjust based on face
        }
    }

    glBindVertexArray(0); // Unbind the VAO
}
