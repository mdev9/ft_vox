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
    void render(GLuint shaderID);
    void setupCube();

private:
    GLuint VAO, VBO, EBO;
    Texture texture;
};

#endif // CUBE_HPP
