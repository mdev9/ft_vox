#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cube {
public:
    Cube(GLuint texture);
    void render(unsigned int shaderID);

private:
    void setupCube();

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture;
};

#endif // CUBE_HPP
