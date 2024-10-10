#ifndef MAIN_HPP
#define MAIN_HPP

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_NAME "ft_vox"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>          
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Function to compile shaders and create shader program
unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource);

#endif // MAIN_HPP
