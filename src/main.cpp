#include "main.hpp"
#include "ShaderProgram.hpp"
#include "InputHandler.hpp"
#include "Camera.hpp"
#include "Cube.hpp"
#include "Texture.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
double lastX = WINDOW_WIDTH / 2.0;
double lastY = WINDOW_HEIGHT / 2.0;
bool firstMouse = true;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	(void) window;
    static double lastX = WINDOW_WIDTH / 2.0;
    static double lastY = WINDOW_HEIGHT / 2.0;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

int main() {
    float deltaTime, lastFrame;
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    ShaderProgram shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    Texture grassSideTexture("assets/grass_side.png");
    Texture grassTopTexture("assets/grass_top.png");
    Texture dirtTexture("assets/dirt.png");
    Texture cobbleTexture("assets/cobblestone.png");

    // Create cubes
    Cube cobbleCube(cobbleTexture);
    Cube dirtCube(dirtTexture);
    Cube grassCube(grassSideTexture, grassTopTexture, dirtTexture);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        InputHandler::processInput(window, camera, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // Setup projection and view matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();

        GLint projLoc = glGetUniformLocation(shader.getID(), "projection");
        GLint viewLoc = glGetUniformLocation(shader.getID(), "view");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Render multiple cubes with different positions
        glm::mat4 model = glm::mat4(1.0f); // Identity matrix
        GLint modelLoc = glGetUniformLocation(shader.getID(), "model");

        // First Cube (Cobble) - Positioned at (0, 0, 0)
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cobbleCube.render(shader.getID());

        // Second Cube (Dirt) - Positioned at (2, 0, 0)
        model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)); // Move to the right
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dirtCube.render(shader.getID());

        // Third Cube (Grass) - Positioned at (-2, 0, 0)
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f)); // Move to the left
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        grassCube.render(shader.getID());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
