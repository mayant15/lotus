#include "EditorCamera.h"

#include <lotus/ecs/components/CCamera.h>
#include <lotus/Input.h>

namespace Editor
{
    using namespace Lotus;

    static constexpr float MovementSpeed = 10.0f;
    static Entity entity;

    void SetupEditorCamera(const Lotus::SceneLoadEvent& event)
    {
        entity = event.pScene->CreateEntity(false);
        auto& camera = entity.AddComponent<CCamera>();
        camera.IsActive = true;
        camera.FOV = 45;

        auto& tf = entity.AddComponent<CTransform>();
        tf.Position = {-5.0, 5.0, 5.0 };
        tf.Rotation = { -35.0, -45.0, 0.0 };
    }

    void MoveCamera(double deltaTime)
    {
        auto& tf = entity.GetComponent<CTransform>();
        Vector3f front = GetForwardVector(tf);
        Vector3f right = GetRightVector(tf);
        float velocity = MovementSpeed * deltaTime;

        if (Input::GetKeyPressed(L_KEY_W))
            tf.Position += front * velocity;
        if (Input::GetKeyPressed(L_KEY_S))
            tf.Position -= front * velocity;
        if (Input::GetKeyPressed(L_KEY_A))
            tf.Position -= right * velocity;
        if (Input::GetKeyPressed(L_KEY_D))
            tf.Position += right * velocity;
        if (Input::GetKeyPressed(L_KEY_E))
            tf.Position += UP * velocity;
        if (Input::GetKeyPressed(L_KEY_Q))
            tf.Position -= UP * velocity;
    }

    void RotateCamera(float xOffset, float yOffset)
    {
        auto& tf = entity.GetComponent<CTransform>();

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        float finalPitch = tf.Rotation.x += yOffset;
        if (finalPitch > 89.0f)
            finalPitch = 89.0f;
        if (finalPitch < -89.0f)
            finalPitch = -89.0f;

        tf.Rotation.x = finalPitch;
        tf.Rotation.y += xOffset;
    }
}
