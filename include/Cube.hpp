#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/glew.h>
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cube {
public:
    Cube(const Texture& texture);
    Cube(const Texture& textureTop, const Texture& textureSide, const Texture& textureBottom);
    void render(GLuint shaderID);
    void setupCube();

private:
    GLuint VAO, VBO, EBO;
	bool isMultiTexture;
	Texture topTexture;
    Texture bottomTexture;
	Texture texture;
};

#endif // CUBE_HPP
