#pragma once

#include "lotus/lotus.h"

constexpr float MouseSensitivity = 0.1f;
constexpr float MovementSpeed = 10.0f;

namespace CameraSystem
{
    using namespace Lotus;

    void OnUpdate(const UpdateEvent& event)
    {
        // Find the camera in the scene
        auto view = GetRegistry()->view<CCamera, CTransform>();
        for (auto entity : view)
        {
            const auto& [camera, transform] = view.get<CCamera, CTransform>(entity);
            if (camera.IsActive)
            {
                Vector3f front = GetForwardVector(transform);
                Vector3f right = GetRightVector(transform);
                float velocity = MovementSpeed * event.DeltaTime;

                if (Input::GetKeyPressed(L_KEY_W))
                    transform.Position += front * velocity;
                if (Input::GetKeyPressed(L_KEY_S))
                    transform.Position -= front * velocity;
                if (Input::GetKeyPressed(L_KEY_A))
                    transform.Position -= right * velocity;
                if (Input::GetKeyPressed(L_KEY_D))
                    transform.Position += right * velocity;
                if (Input::GetKeyPressed(L_KEY_E))
                    transform.Position += UP * velocity;
                if (Input::GetKeyPressed(L_KEY_Q))
                    transform.Position -= UP * velocity;
            }
        }
    }

    void OnMouseEvent(const MouseEvent& event)
    {
        auto view = GetRegistry()->view<CCamera, CTransform>();
        for (auto entity : view)
        {
            const auto& [camera, transform] = view.get<CCamera, CTransform>(entity);
            if (camera.IsActive)
            {
                auto[xOffset, yOffset] = Input::GetMouseDelta();
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
}
