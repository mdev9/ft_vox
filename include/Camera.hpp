#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_SPEED 1.0f

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    void processKeyboard(int direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);

    glm::mat4	getViewMatrix();
	float		getZoom() const { return zoom; }

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
	float zoom;

    void updateCameraVectors();
};

#endif // CAMERA_H
