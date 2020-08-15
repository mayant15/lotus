#pragma once

#include "lotus/lotus.h"
#include "lotus/debug.h"

using namespace Lotus;

void OnKeyboardInput(const KeyboardEvent& event)
{
//    if (event.State != L_KEY_RELEASE)
//    {
        // Find the camera in the scene
        const URef<Scene>& scene = SceneManager::Get().GetActiveScene();
        ACamera camera = scene->GetActiveCamera();
        CTransform& transform =  camera.GetTransform();
        Vector3f front = camera.GetForwardVector();
        Vector3f right = camera.GetRightVector();

        // TODO: Implement deltaTime
        float deltaTime = 1.0f / 60;
        float velocity = 10.0f * deltaTime;
        if (event.KeyCode == L_KEY_W)
            transform.Position += front * velocity;
        if (event.KeyCode == L_KEY_S)
            transform.Position -= front * velocity;
        if (event.KeyCode == L_KEY_A)
            transform.Position -= right * velocity;
        if (event.KeyCode == L_KEY_D)
            transform.Position += right * velocity;
        if (event.KeyCode == L_KEY_E)
            transform.Position += UP * velocity;
        if (event.KeyCode == L_KEY_Q)
            transform.Position -= UP * velocity;
//    }
}

class CameraSystem // : public ISystem
{
public:
    CameraSystem()
    {
        EventManager& em = EventManager::Get();
        em.Bind<KeyboardEvent, &OnKeyboardInput>();
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

