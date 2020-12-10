#pragma once

#include "lotus/lotus.h"

void setup()
{
    using namespace Lotus;

    // Create the shaders to be used
    auto shader = LoadAsset<Shader, ShaderLoader>(SHADER_PBR);

    // Initialize and import the model
    auto model = LoadAsset<Model, ModelLoader>(RESOURCE("mesh/sphere.json"));
    auto planeModel = LoadAsset<Model, ModelLoader>(RESOURCE("mesh/plane.json"));
    auto hdri = LoadAsset<HDRI, HDRILoader>(RESOURCE("skybox/klopp.hdr"));

    // Sky
    auto sky = CreateEntity();
    CSkybox cSkybox;
    cSkybox.Map = hdri;
    sky.AddComponent<CSkybox>(cSkybox);

    CTransform transform;
    CMeshRenderer meshRenderer;

    // Sphere
    auto entity = CreateEntity();
    transform.Position = 1.0f * Y_AXIS;
    transform.Scale = Vector3f {0.3f, 0.3f, 0.3f}; // The sphere was 1m radius, is now 0.3m
    entity.AddComponent<CTransform>(transform);
    transform.Scale = Vector3f {1.0f}; // Reset scale

    meshRenderer.Shader = shader;
    meshRenderer.Model = model;
    entity.AddComponent<CMeshRenderer>(meshRenderer);

    CSphereCollider collider;
    collider.Radius = 0.3f;
    entity.AddComponent<CSphereCollider>(collider);

    CRigidBody rb;
    rb.Gravity = 1.0f;
    rb.IsKinematic = false;
    entity.AddComponent<CRigidBody>(rb);

    // Plane
    auto plane = CreateEntity();
    transform.Position = -1.2f * Y_AXIS;
    plane.AddComponent<CTransform>(transform);

    meshRenderer.Shader = shader;
    meshRenderer.Model = planeModel;
    plane.AddComponent<CMeshRenderer>(meshRenderer);

    CBoxCollider boxCollider;
    boxCollider.Dimensions = Vector3f {3.0, 0.05f, 3.0f};
    plane.AddComponent<CBoxCollider>(boxCollider);

    rb.Gravity = 1.0f;
    rb.IsKinematic = true;
    plane.AddComponent<CRigidBody>(rb);
}
