#pragma once

#include "lotus/lotus.h"

void setup()
{
    auto& assetRegistry = AssetRegistry::Get();

    // Create the shaders to be used
    auto shader = assetRegistry.LoadShader(SHADER_PBR);

    // Initialize and import the model
    auto model = assetRegistry.LoadModel(RESOURCE("mesh/sphere.json"));
    auto planeModel = assetRegistry.LoadModel(RESOURCE("mesh/plane.json"));
    auto cubemap = assetRegistry.LoadCubemap(RESOURCE("skybox/klopp.hdr"));
    // TODO: Load scene from file

    // Sky
    auto sky = CreateEntity<Entity>();
    CSkybox cSkybox;
    cSkybox.Map = cubemap;
    sky.AddComponent<CSkybox>(cSkybox);

    // Sphere
    auto entity = CreateEntity<Entity>();
    CTransform transform;
    transform.Position = 2.0f * Y_AXIS;
    entity.AddComponent<CTransform>(transform);

    CMeshRenderer meshRenderer;
    meshRenderer.Shader = shader;
    meshRenderer.Model = model;
    entity.AddComponent<CMeshRenderer>(meshRenderer);

    // CSphereCollider collider;
    // collider.Radius = 2.0f;
    // entity.AddComponent<CSphereCollider>(collider);
    //
    // CRigidBody rb;
    // rb.Gravity = 1.0f;
    // rb.IsKinematic = false;
    // entity.AddComponent<CRigidBody>(rb);
    //
    // // Plane
    // AActor plane = AActor::Create(-1.2f * Y_AXIS);
    // CMeshRenderer planeRenderer;
    // planeRenderer.Shader = shader;
    // planeRenderer.Model = planeModel;
    // plane.AddComponent<CMeshRenderer>(planeRenderer);
    //
    // // Directional light
    // AActor dirLight = AActor::Create(5.0f * X_AXIS);
    // CDirectionalLight cDirectionalLight;
    // cDirectionalLight.direction = Vector3f(-1.0f, -1.0f, 0.5f);
    // dirLight.AddComponent<CDirectionalLight>(cDirectionalLight);
    //
    // // Spotlight
    // AActor spotlight = AActor::Create(3.0f * Y_AXIS);
    // CSpotlight cSpotlight;
    // cSpotlight.position = 3.0f * Y_AXIS;
    // cSpotlight.direction = -1.0f * Y_AXIS;
    // cSpotlight.innerCutOff = 30;
    // cSpotlight.outerCutOff = 45;
    // spotlight.AddComponent<CSpotlight>(cSpotlight);
    //
    // // Point light
    // AActor pointLight = AActor::Create(2.0f * X_AXIS);
    // CPointLight cPointLight;
    // cPointLight.position = 2.0f * X_AXIS;
    // cPointLight.diffuse = Vector3f(1.0f);
    // pointLight.AddComponent<CPointLight>(cPointLight);
    //
    // AActor pointLight2 = AActor::Create(-2.0f * X_AXIS);
    // CPointLight cPointLight2;
    // cPointLight2.position = -2.0f * X_AXIS;
    // cPointLight2.diffuse = Vector3f(1.0f);
    // pointLight2.AddComponent<CPointLight>(cPointLight2);
    //
    // AActor pointLight3 = AActor::Create(2.0f * Z_AXIS);
    // CPointLight cPointLight3;
    // cPointLight3.position = 2.0f * Z_AXIS;
    // cPointLight3.diffuse = Vector3f(1.0f);
    // pointLight3.AddComponent<CPointLight>(cPointLight3);
    //
    // AActor pointLight4 = AActor::Create(-2.0f * Z_AXIS);
    // CPointLight cPointLight4;
    // cPointLight4.position = -2.0f * Z_AXIS;
    // cPointLight4.diffuse = Vector3f(1.0f);
    // pointLight4.AddComponent<CPointLight>(cPointLight4);

    // Camera
    auto camera = CreateEntity<Entity>();
    CTransform transform1;
    transform1.Position = 10.0f * Z_AXIS + 5.0f * Y_AXIS;
    transform1.Rotation = Vector3f(-20, -90, 0);
    camera.AddComponent<CTransform>(transform1);

    CCamera cameraComponent;
    cameraComponent.FOV = 45;
    cameraComponent.IsActive = true;
    camera.AddComponent<CCamera>(cameraComponent);
}