#pragma once

#include <vector>
#include <string>
#include <memory>

#include "core.h"
#include "components.h"

// Forward declare required classes
namespace Lotus::Rendering
{
    class Shader;

    typedef std::shared_ptr<Shader> SRefShader;
}

namespace Lotus::Resource
{
    class Model;

    typedef std::shared_ptr<Model> SRefModel;
}

namespace Lotus
{
    class LCamera : LObject
    {
        glm::vec3 position;

        // Local vectors
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;

        float yaw = -90.0f;
        float pitch = 0.0f;

        float movementSpeed = 2.5f;
        float mouseSensitivity = 0.1f;
        float zoom = 45.0f;
        float fieldOfView = 45.0f;

    public:
        LCamera(glm::vec3 position_);

        void start() override;

        void update() override;

        glm::mat4 GetViewMatrix() const;

        glm::vec3 getPosition() const;

        glm::vec3 getFront() const;

        glm::vec3 getRight() const;

        glm::vec3 getUp() const;

        float getFieldOfView() const;

        void ProcessKeyboard(glm::vec3 direction, float deltaTime);

        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

        void ProcessMouseScroll(float yoffset);

    private:
        void updateCameraVectors();
    };

    typedef std::shared_ptr<LCamera> SRefCamera;

    struct CModel : IComponent
    {
        Resource::SRefModel model;
        Rendering::SRefShader shader;
    };

    class Actor : public LObject
    {
    protected:
        CTransform transform;
        CModel model;

    public:
        Actor(glm::vec3 position_, const Resource::SRefModel& model_, const Rendering::SRefShader& shader_);

        void update() override;

        void start() override;

        const CModel& getModel() const;

        const CTransform& getTransform() const;
    };

    typedef std::shared_ptr<Actor> SRefActor;

    class ALight : public Actor
    {
    public:
        CPointLight light;

        ALight(glm::vec3 position_, const Resource::SRefModel& model_, const Rendering::SRefShader& shader_);
    };

    typedef std::shared_ptr<ALight> SRefALight;

    class Scene
    {
        SRefCamera camera;
        std::vector<SRefActor> actors;
        std::vector<SRefALight> lights;

    public:
        void addCamera(SRefCamera& camera_);

        void addActor(const SRefActor& actor);

        void addLight(const SRefALight& light);

        const SRefCamera& getCamera() const;

        const std::vector<SRefActor>& getActors() const;

        const std::vector<SRefALight>& getLights() const;
    };

}
