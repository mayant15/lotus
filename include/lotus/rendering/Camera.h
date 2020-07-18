#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

class Camera
{

    // Initialized
    glm::vec3 position{};
    glm::vec3 worldUp{};

    // Computed
    glm::vec3 front{};
    glm::vec3 up{};
    glm::vec3 right{};

    float yaw = -90.0f;
    float pitch = 0.0f;

    float movementSpeed = 2.5f;
    float mouseSensitivity = 0.1f;
    float zoom = 45.0f;
    float fieldOfView = 45.0f;

public:
    explicit Camera(glm::vec3 posVec, glm::vec3 worldUpVec = glm::vec3(0.0f, 1.0f, 0.0f));

    [[nodiscard]] glm::mat4 GetViewMatrix() const;
    [[nodiscard]] glm::vec3 getPosition() const;
    [[nodiscard]] glm::vec3 getFront() const;
    [[nodiscard]] glm::vec3 getRight() const;
    [[nodiscard]] glm::vec3 getUp() const;
    [[nodiscard]] float getFieldOfView() const;

    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};
