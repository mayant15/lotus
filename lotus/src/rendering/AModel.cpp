#include "lotus/rendering.h"

namespace Lotus
{
    AModel::AModel(const Vector3f& position_, const SRefModel& model_, const SRefShader& shader_) : AActor(position_)
    {
        model.model = model_;

        // TODO: Cache these shaders somewhere? So that lighting and camera properties are set only once?
        model.shader = shader_;
    }

    void AModel::update()
    {
        Renderer& renderer = Renderer::get();
        SRefShader shader = model.shader;
        shader->use();



        // Set lighting
        // TODO: Lighting
//        shader->setPointLightArray("pointLight", pointLights);
//        shader->setSpotlightArray("spotlight", spotlights);
//        shader->setDirLightArray("dirLight", dirLights);

        // Set transforms and draw actor
        Matrix4f modelMatrix(1.0f);
        modelMatrix = Lotus::LTranslate(modelMatrix, transform.position);
        modelMatrix = Lotus::LRotate(modelMatrix, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = Lotus::LRotate(modelMatrix, transform.rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = Lotus::LRotate(modelMatrix, transform.rotation.z, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = Lotus::LScale(modelMatrix, transform.scale);
        shader->setMat4f("model", false, modelMatrix);

        // Material. Diffuse is set through a texture
        Vector3f specular(0.5f);
        shader->setVec3f("material.specular", specular);
        shader->setFloat("material.shininess", 32);
        renderer.renderModel(model.model, model.shader);
    }

    void AModel::start()
    {
        AActor::start();
    }
}
