#pragma once

#include "lotus/lotus.h"
#include "lotus/debug.h"

using namespace Lotus;

class CameraSystem
{
public:
    void OnUpdate(const UpdateEvent& event) {
        // Find the camera in the scene
        ACamera camera = SceneManager::Get().GetActiveCamera();
        CTransform& transform =  camera.GetTransform();
        Vector3f front = camera.GetForwardVector();
        Vector3f right = camera.GetRightVector();

        float velocity = 10.0f * event.DeltaTime;
        Input& input = Input::Get();
        if (input.GetKeyPressed(L_KEY_W))
            transform.Position += front * velocity;
        if (input.GetKeyPressed(L_KEY_S))
            transform.Position -= front * velocity;
        if (input.GetKeyPressed(L_KEY_A))
            transform.Position -= right * velocity;
        if (input.GetKeyPressed(L_KEY_D))
            transform.Position += right * velocity;
        if (input.GetKeyPressed(L_KEY_E))
            transform.Position += UP * velocity;
        if (input.GetKeyPressed(L_KEY_Q))
            transform.Position -= UP * velocity;
    }
};

//    void processMouseScroll(float yoffset)
//    {
//        zoom -= (float) yoffset;
//        if (zoom < 1.0f)
//            zoom = 1.0f;
//        if (zoom > 45.0f)
//            zoom = 45.0f;
//    }

//    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
//    {
//        xoffset *= mouseSensitivity;
//        yoffset *= mouseSensitivity;
//
//        yaw += xoffset;
//        pitch += yoffset;
//
//        // make sure that when pitch is out of bounds, screen doesn't get flipped
//        if (constrainPitch)
//        {
//            if (pitch > 89.0f)
//                pitch = 89.0f;
//            if (pitch < -89.0f)
//                pitch = -89.0f;
//        }
//
//        // update Front, Right and Up Vectors using the updated Euler angles
//        updateCameraVectors();
//    }

