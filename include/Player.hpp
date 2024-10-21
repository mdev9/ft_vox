#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Camera.hpp"
#include <glm/glm.hpp>

class VoxelEngine;

class Player : public Camera {
public:
    Player(VoxelEngine* engine, const glm::vec3& position = PLAYER_POS, float yaw = -90.0f, float pitch = 0.0f);

    void update();
    void mouseControl();
    void keyboardControl();
	void scrollControl(double yoffset);

	void moveForward(float velocity);
	void moveBack(float velocity);
	void moveRight(float velocity);
	void moveLeft(float velocity);

private:
    VoxelEngine* engine;
};

#endif
