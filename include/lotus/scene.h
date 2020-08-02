#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "core.h"
#include "physics.h"

enum class ELight
{
    SPOT,
    DIR,
    POINT
};

namespace Lotus
{
    struct CTransform : IComponent
    {
        Vector3f position = Vector3f(0.0f, 0.0f, 0.0f);
        // In Degrees
        Vector3f rotation = Vector3f(0.0f, 0.0f, 0.0f);
        Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f);
    };

    struct CLight : IComponent
    {
        Vector3f ambient = Vector3f(1.0f);
        Vector3f diffuse = Vector3f(1.0f);
        Vector3f specular = Vector3f(1.0f);
    };

    struct CPointLight : CLight
    {
        Vector3f position = Vector3f(0.0f);
        float constant = 1.0f;
        float linear = 1.0f;
        float quadratic = 1.0f;
    };

    struct CDirectionalLight : CLight
    {
        Vector3f direction = Vector3f(0.0f);
    };

    struct CSpotlight : CLight
    {
        Vector3f position = Vector3f(0.0f);
        Vector3f direction = Vector3f(0.0f);
        float constant = 1.0f;
        float linear = 1.0f;
        float quadratic = 1.0f;
        float innerCutOff = 1.0f;
        float outerCutOff = 1.0f;
    };

    class AActor : public LObject
    {
    protected:
        std::vector<std::shared_ptr<AActor>> children;

    public:
        CTransform transform;

        AActor(Vector3f position_);

        void update() override;

        void start() override;

        void addChild(const std::shared_ptr<AActor>& child);
    };

    typedef std::shared_ptr<AActor> SRefAActor;

    class ACamera : AActor
    {
        // Local vectors
        Vector3f front;
        Vector3f up;
        Vector3f right;

        float yaw = -90.0f;
        float pitch = 0.0f;

        float movementSpeed = 2.5f;
        float mouseSensitivity = 0.1f;
        float zoom = 45.0f;
        float fieldOfView = 45.0f;

    public:
        ACamera(Vector3f position_);

        void start() override;

        void update() override;

        Matrix4f GetViewMatrix() const;

        Vector3f getPosition() const;

        Vector3f getFront() const;

        Vector3f getRight() const;

        Vector3f getUp() const;

        float getFieldOfView() const;

        void ProcessKeyboard(Vector3f direction, float deltaTime);

        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

        void ProcessMouseScroll(float yoffset);

    private:
        void updateCameraVectors();
    };

    typedef std::shared_ptr<ACamera> SRefACamera;

    class ALight : public AActor
    {
    public:
        CSpotlight light;

        ALight(const CSpotlight& light_);
    };

    typedef std::shared_ptr<ALight> SRefALight;

    class Scene
    {
    private:
        SRefAActor root;
    public:
        Scene();
        void update();
        SRefAActor getRoot() const;
    };
}
