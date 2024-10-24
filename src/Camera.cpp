#include "Camera.hpp"
#include "Settings.hpp"

Camera::Camera(const glm::vec3& position, float yaw, float pitch) : 
	position(position), yaw(glm::radians(yaw)), pitch(glm::radians(pitch)),
	up(glm::vec3(0, 1, 0)), forward(glm::vec3(0, 0, -1)), right(glm::vec3(1, 0, 0)) {

	m_proj = glm::perspective(V_FOV, DEFAULT_ASPECT_RATIO, NEAR, FAR);
	m_view = glm::mat4();

	playerSpeed = DEFAULT_PLAYER_SPEED;
	speedAdjustment = SPEED_ADJUSTMENT_AMOUNT;

	update();
}

void Camera::update() {
	updateVectors();
	updateViewMatrix();
}

void Camera::updateViewMatrix() {
	m_view = glm::lookAt(position, position + forward, up);
}

void Camera::updateVectors() {
	forward.x = cos(yaw) * cos(pitch);
	forward.y = sin(pitch);
	forward.z = sin(yaw) * cos(pitch);

	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
	up = glm::normalize(glm::cross(right, forward));
}

void Camera::rotatePitch(float delta_y) {
	pitch -= delta_y;
	pitch = glm::clamp(pitch, -PITCH_MAX, PITCH_MAX);
}

void Camera::rotateYaw(float delta_x) {
	yaw += delta_x;
}

void Camera::move(const glm::vec3& direction, float velocity) {
	position += direction * velocity;
}

void Camera::toggleSpeedMultiplier() {
	if (speedMultiplier == 1.0f)
		speedMultiplier = 20.0f;
	else
		speedMultiplier = 1.0f;
	setSpeed();
}

void Camera::setSpeed() {
	playerSpeed = (DEFAULT_PLAYER_SPEED + speedAdjustment) * speedMultiplier;
}

void Camera::setAspectRatio(float aspectRatio) {
    m_proj = glm::perspective(V_FOV, aspectRatio, NEAR, FAR);
}
