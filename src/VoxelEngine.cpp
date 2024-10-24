#include "VoxelEngine.hpp"
#include "Settings.hpp"
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

	WIN_RES = DEFAULT_WIN_RES;
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
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

	onInit();
}

VoxelEngine::~VoxelEngine()
{
	delete player;
	delete shaderProgram;
	delete scene;

    glfwDestroyWindow(window);
    glfwTerminate();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	(void) xoffset;
    VoxelEngine* engine = static_cast<VoxelEngine*>(glfwGetWindowUserPointer(window));
    Player* player = engine->player;
    if (player)
        player->scrollControl(yoffset);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	(void) scancode, (void) mods;
    VoxelEngine* engine = static_cast<VoxelEngine*>(glfwGetWindowUserPointer(window));
    if (engine && key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        engine->toggleFullscreen();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    
    VoxelEngine* engine = static_cast<VoxelEngine*>(glfwGetWindowUserPointer(window));
    if (engine && engine->player) {
        float aspectRatio = static_cast<float>(width) / height;
        engine->player->setAspectRatio(aspectRatio);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	(void) mods;
    VoxelEngine* engine = static_cast<VoxelEngine*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS) {
        VoxelHandler* voxelHandler = engine->scene->getWorld()->getVoxelHandler();
        
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            voxelHandler->setVoxel();
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            voxelHandler->switchMode();
        }
    }
}


void VoxelEngine::onInit()
{
	texture = new Textures();
    player = new Player(this);
    shaderProgram = new ShaderProgram(player);
    scene = new Scene(this);

    glfwSetWindowUserPointer(window, this);
    
    // Register the scroll callback
    glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	// Register the window callbacks
    glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void VoxelEngine::toggleFullscreen() {
    if (isFullscreen) {
        glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, WIN_RES.x, WIN_RES.y, 0);
        isFullscreen = false;
    } else {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

        glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
		int windowedWidth, windowedHeight;
        glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

        WIN_RES = glm::vec2(windowedWidth, windowedHeight);

        glfwSetWindowMonitor(window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        isFullscreen = true;
    }

    player->setAspectRatio(WIN_RES.x / WIN_RES.y);
    glfwSwapInterval(1);
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
