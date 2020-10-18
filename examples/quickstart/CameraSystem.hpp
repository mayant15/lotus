#pragma once

#include "lotus/lotus.h"

using namespace Lotus;

constexpr float MouseSensitivity = 0.1f;
constexpr float MovementSpeed = 10.0f;

class CameraSystem
{
public:
    static void OnUpdate(const UpdateEvent& event)
    {
        // Find the camera in the scene
//        ECS::ACamera camera = GET(SceneManager).GetActiveCamera();
//        CTransform& transform = camera.GetTransform();
//        Vector3f front = camera.GetForwardVector();
//        Vector3f right = camera.GetRightVector();
//        float velocity = MovementSpeed * event.DeltaTime;
//
//        Input& input = Input::Get();
//        if (input.GetKeyPressed(L_KEY_W))
//            transform.Position += front * velocity;
//        if (input.GetKeyPressed(L_KEY_S))
//            transform.Position -= front * velocity;
//        if (input.GetKeyPressed(L_KEY_A))
//            transform.Position -= right * velocity;
//        if (input.GetKeyPressed(L_KEY_D))
//            transform.Position += right * velocity;
//        if (input.GetKeyPressed(L_KEY_E))
//            transform.Position += UP * velocity;
//        if (input.GetKeyPressed(L_KEY_Q))
//            transform.Position -= UP * velocity;
    }

    static void OnMouseEvent(const MouseEvent& event)
    {
        auto view = GetRegistry()->view<CCamera, CTransform>();
        for (auto entity : view)
        {
            const auto& [camera, transform] = view.get<CCamera, CTransform>(entity);
            if (camera.IsActive)
            {
                Input& input = Input::Get();
                auto[xOffset, yOffset] = input.GetMouseDelta();
                xOffset *= MouseSensitivity;
                yOffset *= MouseSensitivity;

                // make sure that when pitch is out of bounds, screen doesn't get flipped
                float finalPitch = transform.Rotation.x += yOffset;
                if (finalPitch > 89.0f)
                    finalPitch = 89.0f;
                if (finalPitch < -89.0f)
                    finalPitch = -89.0f;

                transform.Rotation.x = finalPitch;
                transform.Rotation.y += xOffset;
                break;
            }
        }
    }
};
