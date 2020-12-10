#pragma once

#include "lotus/lotus.h"

void setup()
{
    using namespace Lotus;
    auto& assetRegistry = GET(AssetRegistry);

    // Create the shaders to be used
    auto shader = assetRegistry.LoadShader(SHADER_PBR);

    // Initialize and import the model
    auto model = assetRegistry.LoadModel(RESOURCE("mesh/sphere.json"));
    auto planeModel = assetRegistry.LoadModel(RESOURCE("mesh/plane.json"));
    auto hdri = assetRegistry.LoadHDRI(RESOURCE("skybox/klopp.hdr"));
    // TODO: Load scene from file

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
    transform.Scale = Vector3f {0.3f, 0.3f, 0.3f}; // The sphere was 1m radius, is now 0.2m
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

    // Directional light
    auto dirLight = CreateEntity();
    transform.Rotation = Vector3f (-45.0f, -45.0f, 0.0f);
    dirLight.AddComponent<CTransform>(transform);
    dirLight.AddComponent<CLight>(CLight {});

    // Spotlight
    auto spotlight = CreateEntity();
    transform.Position = 3.0f * Y_AXIS;
    transform.Rotation = Vector3f (-90.0f, -90.0f, 0.0f);
    spotlight.AddComponent<CTransform>(transform);

    CSpotlight cSpotlight;
    cSpotlight.innerCutOff = 30;
    cSpotlight.outerCutOff = 45;
    spotlight.AddComponent<CSpotlight>(cSpotlight);

    // Point lights
    auto pointLight = CreateEntity(RESOURCE("recipe/pointlight.json"));
    auto& t = pointLight.GetComponent<CTransform>();
    t.Rotation = 2.0f * X_AXIS;

    pointLight = CreateEntity(RESOURCE("recipe/pointlight.json"));
    t = pointLight.GetComponent<CTransform>();
    t.Rotation = -2.0f * X_AXIS;

    pointLight = CreateEntity(RESOURCE("recipe/pointlight.json"));
    t = pointLight.GetComponent<CTransform>();
    t.Rotation = -2.0f * Z_AXIS;

    pointLight = CreateEntity(RESOURCE("recipe/pointlight.json"));
    t = pointLight.GetComponent<CTransform>();
    t.Rotation = 2.0f * Z_AXIS;

    // Camera
    auto camera = CreateEntity(RESOURCE("recipe/camera.json"));
}
