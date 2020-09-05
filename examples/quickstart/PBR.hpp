#pragma once

#include "lotus/lotus.h"

void setup()
{
    auto& assetRegistry = AssetRegistry::Get();

    // Create the shaders to be used
    auto shader = assetRegistry.LoadShader(
        R"(D:\code\lotus\lotus\src\rendering\shaders\pbr.vert)",
        R"(D:\code\lotus\lotus\src\rendering\shaders\pbr.frag)"
    );

    auto skyShader = assetRegistry.LoadShader(
        R"(D:\code\lotus\examples\quickstart\resources\shaders\skybox.vert)",
        R"(D:\code\lotus\examples\quickstart\resources\shaders\skybox.frag)"
    );

    // Initialize and import the model
    auto model = assetRegistry.LoadModel(R"(D:\code\lotus\examples\quickstart\resources\mesh\sphere.json)");
    auto planeModel = assetRegistry.LoadModel(R"(D:\code\lotus\examples\quickstart\resources\mesh\plane.json)");

    std::vector<std::string> faces
    {
        R"(D:\code\lotus\examples\quickstart\resources\skybox\right.jpg)",
        R"(D:\code\lotus\examples\quickstart\resources\skybox\left.jpg)",
        R"(D:\code\lotus\examples\quickstart\resources\skybox\top.jpg)",
        R"(D:\code\lotus\examples\quickstart\resources\skybox\bottom.jpg)",
        R"(D:\code\lotus\examples\quickstart\resources\skybox\front.jpg)",
        R"(D:\code\lotus\examples\quickstart\resources\skybox\back.jpg)",
    };
    Handle<Cubemap> cubemap = assetRegistry.LoadCubemap(faces);

    SceneManager& sceneManager = SceneManager::Get();
    const URef<Scene>& scene = sceneManager.LoadScene(R"(D:\code\lotus\examples\quickstart\resources\scenes\SampleScene.json)");

    // Sky
    // Entity sky = scene->CreateEntity();
    // CSkybox cSkybox;
    // cSkybox.Shader = skyShader;
    // cSkybox.Map = cubemap;
    // sky.AddComponent<CSkybox>(cSkybox);

    // Sphere
    AActor entity = scene->CreateActor(ORIGIN);
    CMeshRenderer meshRenderer;
    meshRenderer.Shader = shader;
    meshRenderer.Model = model;
    entity.AddComponent<CMeshRenderer>(meshRenderer);

    // Plane
    AActor plane = scene->CreateActor(-1.2f * Y_AXIS);
    CMeshRenderer planeRenderer;
    planeRenderer.Shader = shader;
    planeRenderer.Model = planeModel;
    plane.AddComponent<CMeshRenderer>(planeRenderer);

    // Directional light
    AActor dirLight = scene->CreateActor(5.0f * X_AXIS);
    CDirectionalLight cDirectionalLight;
    cDirectionalLight.direction = Vector3f(-1.0f, -1.0f, 0.5f);
    dirLight.AddComponent<CDirectionalLight>(cDirectionalLight);

    // Spotlight
    AActor spotlight = scene->CreateActor(3.0f * Y_AXIS);
    CSpotlight cSpotlight;
    cSpotlight.position = 3.0f * Y_AXIS;
    cSpotlight.direction = -1.0f * Y_AXIS;
    cSpotlight.innerCutOff = 30;
    cSpotlight.outerCutOff = 45;
    spotlight.AddComponent<CSpotlight>(cSpotlight);

    // Point light
    AActor pointLight = scene->CreateActor(2.0f * X_AXIS);
    CPointLight cPointLight;
    cPointLight.position = 2.0f * X_AXIS;
    cPointLight.diffuse = Vector3f(1.0f);
    pointLight.AddComponent<CPointLight>(cPointLight);
    
    AActor pointLight2 = scene->CreateActor(-2.0f * X_AXIS);
    CPointLight cPointLight2;
    cPointLight2.position = -2.0f * X_AXIS;
    cPointLight2.diffuse = Vector3f(1.0f);
    pointLight2.AddComponent<CPointLight>(cPointLight2);
    
    AActor pointLight3 = scene->CreateActor(2.0f * Z_AXIS);
    CPointLight cPointLight3;
    cPointLight3.position = 2.0f * Z_AXIS;
    cPointLight3.diffuse = Vector3f(1.0f);
    pointLight3.AddComponent<CPointLight>(cPointLight3);
    
    AActor pointLight4 = scene->CreateActor(-2.0f * Z_AXIS);
    CPointLight cPointLight4;
    cPointLight4.position = -2.0f * Z_AXIS;
    cPointLight4.diffuse = Vector3f(1.0f);
    pointLight4.AddComponent<CPointLight>(cPointLight4);

    // Camera
    ACamera camera = scene->CreateCamera(10.0f * Z_AXIS + 5.0f * Y_AXIS, 45, true);
    camera.GetTransform().Rotation = Vector3f(-20, -90, 0);
}