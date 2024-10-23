#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Settings.hpp"

class Camera {
	public:
		glm::vec3 position;
		float yaw, pitch;

		glm::vec3 up, forward, right;
		glm::mat4 m_proj, m_view;

		Camera(const glm::vec3& position, float yaw, float pitch);

		void update();
		void updateViewMatrix();
		void updateVectors();
		void rotatePitch(float delta_y);
		void rotateYaw(float delta_x);
		void move(const glm::vec3& direction, float velocity);
		void toggleSpeedMultiplier();
		void setSpeed(float speed);
		void setAspectRatio(float aspectRatio);

	protected:
		float speedMultiplier = 1.0f;
		float playerSpeed = 5.0f;
};

#endif
