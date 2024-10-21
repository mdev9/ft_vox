#include "Player.hpp"
#include "Settings.hpp"
#include "VoxelEngine.hpp"
#include <GLFW/glfw3.h>

Player::Player(VoxelEngine* engine, const glm::vec3& position, float yaw, float pitch)
	: Camera(position, yaw, pitch), engine(engine) {}

void Player::update() {
    keyboardControl();
    mouseControl();
    Camera::update();
}

void Player::scrollControl(double yoffset) {
	if (yoffset > 0)
		setSpeed(PLAYER_SPEED + SPEED_ADJUSTMENT_AMOUNT);
	else if (yoffset < 0)
		setSpeed(PLAYER_SPEED - SPEED_ADJUSTMENT_AMOUNT);
}

void Player::mouseControl() {
    double mouseX, mouseY;

    glfwGetCursorPos(engine->window, &mouseX, &mouseY);
    glfwSetCursorPos(engine->window, WIN_RES.x / 2, WIN_RES.y / 2); // Reset mouse to center of screen

    float deltaX = static_cast<float>(mouseX - WIN_RES.x / 2);
    float deltaY = static_cast<float>(mouseY - WIN_RES.y / 2);

    if (deltaX != 0)
        rotateYaw(deltaX * MOUSE_SENSITIVITY);
    if (deltaY != 0)
        rotatePitch(deltaY * MOUSE_SENSITIVITY);
}

void Player::keyboardControl() {
    float velocity = playerSpeed * engine->deltaTime; 

    if (glfwGetKey(engine->window, GLFW_KEY_W) == GLFW_PRESS)
        moveForward(velocity);
    if (glfwGetKey(engine->window, GLFW_KEY_S) == GLFW_PRESS)
        moveBack(velocity);
    if (glfwGetKey(engine->window, GLFW_KEY_D) == GLFW_PRESS)
        moveRight(velocity);
    if (glfwGetKey(engine->window, GLFW_KEY_A) == GLFW_PRESS)
		moveLeft(velocity);
	if (glfwGetKey(engine->window, GLFW_KEY_SPACE) == GLFW_PRESS)
		move(glm::vec3(0, 1, 0), velocity);
	if (glfwGetKey(engine->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		move(glm::vec3(0, -1, 0), velocity);
	if (glfwGetKey(engine->window, GLFW_KEY_F) == GLFW_PRESS)
		toggleSpeedMultiplier();
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
