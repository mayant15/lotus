#include "EditorCamera.h"

#include <lotus/ecs/components/CCamera.h>
#include <lotus/ecs/Entity.h>
#include <lotus/Input.h>

namespace Editor
{
    using namespace Lotus;

    static constexpr float MouseSensitivity = 0.1f;
    static constexpr float MovementSpeed = 10.0f;

    static CCamera* camera = nullptr;
    static CTransform* transform = nullptr;

    void OnSceneLoad(const SceneLoadEvent& event)
    {
        Entity e = CreateEntity();
        camera = &e.AddComponent<CCamera>();
        transform = &e.AddComponent<CTransform>();

        camera->IsActive = true;
        camera->FOV = 45;

        transform->Position = {0.0f, 5.0f, 12.0f};
    }

    void OnUpdate(const Lotus::UpdateEvent& event)
    {
        Vector3f front = GetForwardVector(*transform);
        Vector3f right = GetRightVector(*transform);
        float velocity = MovementSpeed * event.DeltaTime;

        if (Input::GetKeyPressed(L_KEY_W))
            transform->Position += front * velocity;
        if (Input::GetKeyPressed(L_KEY_S))
            transform->Position -= front * velocity;
        if (Input::GetKeyPressed(L_KEY_A))
            transform->Position -= right * velocity;
        if (Input::GetKeyPressed(L_KEY_D))
            transform->Position += right * velocity;
        if (Input::GetKeyPressed(L_KEY_E))
            transform->Position += UP * velocity;
        if (Input::GetKeyPressed(L_KEY_Q))
            transform->Position -= UP * velocity;
    }

    void RotateCamera(float xOffset, float yOffset)
    {
        // make sure that when pitch is out of bounds, screen doesn't get flipped
        float finalPitch = transform->Rotation.x += yOffset;
        if (finalPitch > 89.0f)
            finalPitch = 89.0f;
        if (finalPitch < -89.0f)
            finalPitch = -89.0f;

        transform->Rotation.x = finalPitch;
        transform->Rotation.y += xOffset;
    }
}
