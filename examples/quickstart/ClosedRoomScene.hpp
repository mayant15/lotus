#pragma once

#include "lotus/lotus.h"

void setup()
{
    auto& assetRegistry = AssetRegistry::Get();

    // Create the shaders to be used
    auto shader = assetRegistry.LoadShader(
        R"(D:\code\lotus\examples\quickstart\resources\shaders\standard.vert)",
        R"(D:\code\lotus\examples\quickstart\resources\shaders\diffuse.frag)"
    );

    // Initialize and import the model
    auto model = assetRegistry.LoadModel(R"(D:\code\lotus\examples\quickstart\resources\mesh\sphere.json)");
    auto planeModel = assetRegistry.LoadModel(R"(D:\code\lotus\examples\quickstart\resources\mesh\plane.json)");

    SceneManager& sceneManager = SceneManager::Get();
    const URef<Scene>& scene = sceneManager.LoadScene(R"(D:\code\lotus\examples\quickstart\resources\scenes\SampleScene.json)");

    // Sphere 1
    AActor entity = scene->CreateActor(-3.0f * X_AXIS);
    CMeshRenderer meshRenderer;
    meshRenderer.Shader = shader;
    meshRenderer.Model = model;
    entity.AddComponent<CMeshRenderer>(meshRenderer);

    // Sphere 2
    AActor entity2 = scene->CreateActor(3.0f * X_AXIS);
    // CMeshRenderer meshRenderer;
    // meshRenderer.Shader = shader;
    // meshRenderer.Model = model;
    entity2.AddComponent<CMeshRenderer>(meshRenderer);

    // Plane
    AActor plane = scene->CreateActor(-2.0f * Y_AXIS);
    CMeshRenderer planeRenderer;
    planeRenderer.Shader = shader;
    planeRenderer.Model = planeModel;
    plane.AddComponent<CMeshRenderer>(planeRenderer);

    // background
    AActor plane2 = scene->CreateActor(-8.0f * Z_AXIS);
    CTransform& backTransform = plane2.GetTransform();
    backTransform.Rotation = 90.0f * X_AXIS;
    plane2.AddComponent<CMeshRenderer>(planeRenderer);

    // Directional light
    // AActor dirLight = scene->CreateActor(5.0f * Z_AXIS);
    // CDirectionalLight cDirectionalLight;
    // cDirectionalLight.direction = -1.0f * Z_AXIS;
    // cDirectionalLight.diffuse = Vector3f (1.0f);
    // dirLight.AddComponent<CDirectionalLight>(cDirectionalLight);

    // Spotlight
    AActor spotlight = scene->CreateActor(5.0f * Z_AXIS);
    CSpotlight cSpotlight;
    // cSpotlight.position = 5.0f * Y_AXIS;
    // cSpotlight.direction = -1.0f * Y_AXIS -0.5f * Z_AXIS;
    cSpotlight.position = 5.0f * Z_AXIS;
    cSpotlight.direction = 1.0f * Z_AXIS;
    spotlight.AddComponent<CMeshRenderer>(meshRenderer);
    spotlight.AddComponent<CSpotlight>(cSpotlight);

    // Camera
    ACamera camera = scene->CreateCamera(10.0f * Z_AXIS + 5.0f * Y_AXIS, 45, true);
    camera.GetTransform().Rotation = Vector3f(-20, -90, 0);
}