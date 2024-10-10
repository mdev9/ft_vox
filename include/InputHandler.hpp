#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#define MOUSE_SENSITIVITY 1.0f  // Mouse sensitivity adjustment
#define ARROW_ROTATION_SPEED 2000.0f  // Arrow keys rotation speed

#include "Camera.hpp"

class InputHandler {
public:
    static void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif
