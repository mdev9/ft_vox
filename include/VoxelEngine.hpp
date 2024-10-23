#ifndef VOXEL_ENGINE_HPP
#define VOXEL_ENGINE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Settings.hpp"
#include "Player.hpp"
#include "ShaderProgram.hpp"
#include "Textures.hpp"
#include "Scene.hpp"
#include <iostream>
#include <chrono>

class Scene;

class VoxelEngine {
	public:
		GLFWwindow* window;
		Player* player;
		Textures* texture;
		ShaderProgram* shaderProgram;
		Scene* scene;
		float deltaTime = 0.0f;
		double lastFrameTime = 0.0;
		bool isRunning = true;

		VoxelEngine();
		~VoxelEngine();

		void run();
		void update();
		void render();
		void handleEvents();
		void toggleFullscreen();

	private:
		glm::mat4 proj;
		glm::mat4 view;

		bool isFullscreen = false;
		int windowedPosX, windowedPosY;
		glm::vec2 WIN_RES;

		void onInit();
		void initGlfw();
		void createWindow();
		void setWindowIcon(const char* path);
};

#endif
