#ifndef MAIN_HPP
#define MAIN_HPP

#define STB_IMAGE_IMPLEMENTATION

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define MOUSE_SENSITIVITY 1.0f  // Mouse sensitivity adjustment
#define ARROW_ROTATION_SPEED 2000.0f  // Arrow keys rotation speed


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>
#include "glm/glm/glm.hpp"          
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Camera.hpp"         

// Vertex Shader
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 projection;
uniform mat4 view;
void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

// Fragment Shader
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texture1;
void main() {
    FragColor = texture(texture1, TexCoord);
}
)";

// Function to compile shaders and create shader program
unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource);

#endif // MAIN_HPP
