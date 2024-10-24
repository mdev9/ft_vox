#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

// Resolution
const glm::vec2 DEFAULT_WIN_RES(1600, 900);

// Chunk
const int CHUNK_SIZE = 32;
const int H_CHUNK_SIZE = CHUNK_SIZE / 2;
const int CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
const int CHUNK_VOL = CHUNK_AREA * CHUNK_SIZE;

// World
const int WORLD_W = 10;
const int WORLD_H = 2;
const int WORLD_D = WORLD_W;
const int WORLD_AREA = WORLD_W * WORLD_D;
const int WORLD_VOL = WORLD_AREA * WORLD_H;

// World Center
const int CENTER_XZ = WORLD_W * H_CHUNK_SIZE;
const int CENTER_Y = WORLD_H * H_CHUNK_SIZE;

// Camera settings
const float DEFAULT_ASPECT_RATIO = DEFAULT_WIN_RES.x / DEFAULT_WIN_RES.y;
const float FOV_DEG = 50.0f;
const float V_FOV = glm::radians(FOV_DEG);
const float NEAR = 0.1f;
const float FAR = 2000.0f;
const float PITCH_MAX = glm::radians(89.0f);

// Player settings
const float DEFAULT_PLAYER_SPEED = 5.0f;
const float SPEED_ADJUSTMENT_AMOUNT = 5.0f;
const float PLAYER_ROT_SPEED = 0.003f;
const glm::vec3 PLAYER_POS(CENTER_XZ, WORLD_H * CHUNK_SIZE, CENTER_XZ);
const float MOUSE_SENSITIVITY = 0.002f;

// Colors
const glm::vec3 BG_COLOR(0.1f, 0.16f, 0.25f);

// Icon
const std::string ICON = "assets/grass_side.png";

#endif
