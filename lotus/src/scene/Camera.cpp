#include "lotus/scene.h"
#include "lotus/physics.h"

namespace Lotus
{
    ACamera::ACamera(Vector3f position_) : AActor(position_)
    {
        transform.rotation = Vector3f(0.0f, 180.0f, 0.0f);
        updateCameraVectors();
    }

    void ACamera::updateCameraVectors()
    {
        // calculate the new Front vector
        float x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        float y = sin(glm::radians(pitch));
        float z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = LNormalize(Vector3f(x, y, z));

        // also re-calculate the Right and Up vector
        // normalize the vectors, because their length gets closer to 0 the more you look
        // up or down which results in slower movement.
        right = LNormalize(LCross(front, UP));
        up = LNormalize(LCross(right, front));
    }

    void ACamera::ProcessMouseScroll(float yoffset)
    {
        zoom -= (float) yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

    void ACamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
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

    void ACamera::ProcessKeyboard(Vector3f direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD)
            transform.position += front * velocity;
        if (direction == BACKWARD)
            transform.position -= front * velocity;
        if (direction == LEFT)
            transform.position -= right * velocity;
        if (direction == RIGHT)
            transform.position += right * velocity;
        if (direction == UP)
            transform.position += UP * velocity;
        if (direction == DOWN)
            transform.position -= UP * velocity;
    }

    Matrix4f ACamera::GetViewMatrix() const
    {
        return LLookAt(transform.position, transform.position + front, up);
    }

    Vector3f ACamera::getPosition() const
    {
        return transform.position;
    }

    Vector3f ACamera::getFront() const
    {
        return front;
    }

    Vector3f ACamera::getRight() const
    {
        return right;
    }

    Vector3f ACamera::getUp() const
    {
        return up;
    }

    float ACamera::getFieldOfView() const
    {
        return glm::radians(fieldOfView);
    }

    // TODO:
    void ACamera::start()
    {

    }

    // TODO:
    void ACamera::update()
    {

    }
}