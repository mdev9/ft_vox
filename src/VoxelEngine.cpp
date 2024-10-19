#include "VoxelEngine.hpp"
#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

VoxelEngine::VoxelEngine()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIN_RES.x, WIN_RES.y, "", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	setWindowIcon(ICON.c_str());

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

	glewExperimental = GL_TRUE; 
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "ERROR::GLEW::INITIALIZATION_FAILED: " << glewGetErrorString(err) << std::endl;
		exit(EXIT_FAILURE);
	}

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Set initial state
    deltaTime = 0.0;
    lastFrameTime = glfwGetTime();
    isRunning = true;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE); //more optimized
	glDisable(GL_CULL_FACE); //for testing purposes only
    glEnable(GL_BLEND);

	// Initialize components
	onInit();


	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error: " << err << std::endl;
	}
}

VoxelEngine::~VoxelEngine()
{
	delete player;
	delete shaderProgram;
	delete scene;

    glfwDestroyWindow(window);
    glfwTerminate();
}

void VoxelEngine::onInit()
{
    player = new Player(this);
    shaderProgram = new ShaderProgram(player);
    scene = new Scene(this);
}

void VoxelEngine::update()
{
    // Calculate delta time
    double currentFrameTime = glfwGetTime();
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    player->update();
    shaderProgram->update();
	scene->update();

    // Set window title with FPS
    std::string title = std::to_string(1.0 / deltaTime);
    glfwSetWindowTitle(window, title.c_str());
}

void VoxelEngine::render()
{
    // Clear the screen
    glClearColor(BG_COLOR[0], BG_COLOR[1], BG_COLOR[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->render();

    // Swap buffers
    glfwSwapBuffers(window);
}

void VoxelEngine::handleEvents()
{
    // Poll for events
    glfwPollEvents();

    // Check for window close or ESC key press
    if (glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        isRunning = false;
    }
}

void VoxelEngine::run()
{
    while (isRunning) {
        handleEvents();
        update();
        render();
    }
}

void VoxelEngine::setWindowIcon(const char* iconPath)
{
    int width, height, channels;
    unsigned char* image = stbi_load(iconPath, &width, &height, &channels, 4);  // Load the icon image with 4 channels (RGBA)
    if (image) {
        GLFWimage glfwIcon;
        glfwIcon.width = width;
        glfwIcon.height = height;
        glfwIcon.pixels = image;

        // Set the window icon
        glfwSetWindowIcon(window, 1, &glfwIcon);

        // Free the image after setting the icon
        stbi_image_free(image);
    } else {
        std::cerr << "Failed to load icon image: " << iconPath << std::endl;
    }
}
