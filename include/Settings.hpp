#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

//Resolution
const glm::vec2 WIN_RES(1600, 900);

// Camera settings
const float ASPECT_RATIO = WIN_RES.x / WIN_RES.y;
const float FOV_DEG = 50.0f;
const float V_FOV = glm::radians(FOV_DEG);
const float NEAR = 0.1f;
const float FAR = 2000.0f;
const float PITCH_MAX = glm::radians(89.0f);

// Player settings
const float PLAYER_SPEED = 0.005f;
const float PLAYER_ROT_SPEED = 0.003f;
const glm::vec3 PLAYER_POS(0.0f, 0.0f, 1.0f);
const float MOUSE_SENSITIVITY = 0.002f;

//Colors
const glm::vec3 BG_COLOR(0.1f, 0.16f, 0.25f);

//Icon
const std::string ICON = "assets/grass_side.png";

#endif
