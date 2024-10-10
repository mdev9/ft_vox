// Cube.hpp
#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include <GL/glew.h>

class Cube {
public:
    Cube();
    void draw(GLuint shaderProgram);

private:
    GLuint VAO, VBO;
    void setupCube();
};

#endif // CUBE_H
