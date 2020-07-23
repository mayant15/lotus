#include <glm/ext/matrix_transform.hpp>
#include "lotus/scene.h"
#include "lotus/physics.h"

namespace Lotus
{
    LCamera::LCamera(glm::vec3 position_)
    {
        position = position_;
        updateCameraVectors();
    }

    void LCamera::updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 frontVec;
        frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        frontVec.y = sin(glm::radians(pitch));
        frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(frontVec);

        // also re-calculate the Right and Up vector
        // normalize the vectors, because their length gets closer to 0 the more you look
        // up or down which results in slower movement.
        right = glm::normalize(glm::cross(front, UP));
        up = glm::normalize(glm::cross(right, front));
    }

    void LCamera::ProcessMouseScroll(float yoffset)
    {
        zoom -= (float) yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

    void LCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void LCamera::ProcessKeyboard(glm::vec3 direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
        if (direction == UP)
            position += UP * velocity;
        if (direction == DOWN)
            position -= UP * velocity;
    }

    glm::mat4 LCamera::GetViewMatrix() const
    {
        return glm::lookAt(position, position + front, up);
    }

    glm::vec3 LCamera::getPosition() const
    {
        return position;
    }

    glm::vec3 LCamera::getFront() const
    {
        return front;
    }

    glm::vec3 LCamera::getRight() const
    {
        return right;
    }

    glm::vec3 LCamera::getUp() const
    {
        return up;
    }

    float LCamera::getFieldOfView() const
    {
        return glm::radians(fieldOfView);
    }

    // TODO:
    void LCamera::start()
    {

    }

    // TODO:
    void LCamera::update()
    {

    }
}