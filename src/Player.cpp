#include "Player.hpp"
#include "Settings.hpp"
#include "VoxelEngine.hpp"

Player::Player(VoxelEngine* engine, const glm::vec3& position, float yaw, float pitch)
	: Camera(position, yaw, pitch), engine(engine) {}

void Player::update() {
    keyboardControl();
    mouseControl();
    Camera::update();
}

void Player::mouseControl() {
    double mouseX, mouseY;
    glfwGetCursorPos(engine->window, &mouseX, &mouseY);
    glfwSetCursorPos(engine->window, WIN_RES.x / 2, WIN_RES.y / 2); // Reset mouse to center of screen

    float deltaX = static_cast<float>(mouseX - WIN_RES.x / 2);
    float deltaY = static_cast<float>(WIN_RES.y / 2 - mouseY); // Inverted Y axis

    if (deltaX != 0)
        rotateYaw(deltaX * MOUSE_SENSITIVITY);
    if (deltaY != 0)
        rotatePitch(deltaY * MOUSE_SENSITIVITY);
}

void Player::keyboardControl() {
    float velocity = PLAYER_SPEED * engine->deltaTime; 

    if (glfwGetKey(engine->window, GLFW_KEY_W) == GLFW_PRESS)
        moveForward(velocity);
    if (glfwGetKey(engine->window, GLFW_KEY_S) == GLFW_PRESS)
        moveBack(velocity);
    if (glfwGetKey(engine->window, GLFW_KEY_D) == GLFW_PRESS)
        moveRight(velocity);
    if (glfwGetKey(engine->window, GLFW_KEY_A) == GLFW_PRESS)
        moveLeft(velocity);
    if (glfwGetKey(engine->window, GLFW_KEY_Q) == GLFW_PRESS)
        move(glm::vec3(0, 1, 0), velocity);
    if (glfwGetKey(engine->window, GLFW_KEY_E) == GLFW_PRESS)
        move(glm::vec3(0, -1, 0), velocity);
}

void Player::moveForward(float velocity) {
    move(forward, velocity);
}

void Player::moveBack(float velocity) {
    move(-forward, velocity);
}

void Player::moveRight(float velocity) {
    move(right, velocity);
}

void Player::moveLeft(float velocity) {
    move(-right, velocity);
}
