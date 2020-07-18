#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "lotus/scene/objects.h"

ALight::ALight(glm::vec3 position, const CMesh& meshComponent) : Actor(position, meshComponent)
{
    light.position = position;
    light.diffuse = glm::vec3(1.0f);
    light.ambient = glm::vec3(0.2f);
    light.specular = glm::vec3(0.5f);
    light.constant = 1.0f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
}

Actor::Actor(glm::vec3 position_, const CMesh& meshComponent_)
{
    transform.position = position_;

    // TODO: What kind of a transfer happens here?
    mesh = meshComponent_;
}

void Actor::start()
{

}

void Actor::update()
{
    std::shared_ptr<Shader> shader = mesh.shader;
    shader->use();

    // Set the matrices as uniforms for our shader.
    glm::mat4 model(1.0f);
    glm::translate(model, transform.position);
    glm::rotate(model, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::rotate(model, transform.rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::rotate(model, transform.rotation.z, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::scale(model, transform.scale);
    shader->setMat4fv("model", GL_FALSE, glm::value_ptr(model));

    // Material. Diffuse is set through a texture
    float specular[] = {0.5f, 0.5f, 0.5f};
    shader->setVec3f("material.specular", specular);
    shader->setFloat("material.shininess", 32);

    mesh.model->draw(*shader);
}

const CMesh& Actor::getMesh() const
{
    return mesh;
}
