#include "lotus/lotus.h"
#include "lotus/debug.h"

#include "CameraSystem.hpp"

int main()
{
    // Have to initialize the engine first
    auto& engine = Engine::Get();
    LotusOp config;
    config.RenderAPI = ERenderAPI::OPEN_GL;
    config.IsDebug = true;
    config.Width = 1024;
    config.Height = 800;
    engine.Initialize(config);

    auto& assetRegistry = AssetRegistry::Get();

    // Create the shaders to be used
    auto shader = assetRegistry.LoadShader(
        R"(D:\code\lotus\examples\quickstart\resources\shaders\standard.vsh)",
        R"(D:\code\lotus\examples\quickstart\resources\shaders\diffuse.fsh)"
    );

    auto skyShader = assetRegistry.LoadShader(
        R"(D:\code\lotus\examples\quickstart\resources\shaders\skybox.vsh)",
        R"(D:\code\lotus\examples\quickstart\resources\shaders\skybox.fsh)"
    );

    // Initialize and import the model
    auto model = assetRegistry.LoadModel(R"(D:\code\lotus\examples\quickstart\resources\mesh\untitled.json)");
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
    Entity sky = scene->CreateEntity();
    CSkybox cSkybox;
    cSkybox.Shader = skyShader;
    cSkybox.Map = cubemap;
    sky.AddComponent<CSkybox>(cSkybox);

    // Box
    AActor entity = scene->CreateActor(ORIGIN);
    CMeshRenderer meshRenderer;
    meshRenderer.Shader = shader;
    meshRenderer.Model = model;
    entity.AddComponent<CMeshRenderer>(meshRenderer);

    // Plane
    AActor plane = scene->CreateActor(-2.0f * Y_AXIS);
    CMeshRenderer planeRenderer;
    planeRenderer.Shader = shader;
    planeRenderer.Model = planeModel;
    plane.AddComponent<CMeshRenderer>(planeRenderer);

    // Directional light
    AActor dirLight = scene->CreateActor(5.0f * X_AXIS);
    CDirectionalLight cDirectionalLight;
    cDirectionalLight.direction = Vector3f(-1.0f, -1.0f, 1.0f);
    dirLight.AddComponent<CDirectionalLight>(cDirectionalLight);

    // Spotlight
    AActor spotlight = scene->CreateActor(5.0f * Y_AXIS);
    CSpotlight cSpotlight;
    cSpotlight.position = 5.0f * Y_AXIS;
    cSpotlight.direction = -1.0f * Y_AXIS;
    spotlight.AddComponent<CSpotlight>(cSpotlight);

    // Point light
    AActor pointLight = scene->CreateActor(3.0f * Y_AXIS - 2.0f * X_AXIS);
    CPointLight cPointLight;
    cPointLight.position = 3.0f * Y_AXIS - 2.0f * X_AXIS;
    pointLight.AddComponent<CPointLight>(cPointLight);

    // Camera
    ACamera camera = scene->CreateCamera(10.0f * Z_AXIS + 5.0f * Y_AXIS, 45, true);
    camera.GetTransform().Rotation = Vector3f(-20, -90, 0);

    // Test out python script
    exec_file(R"(D:\code\lotus\examples\quickstart\resources\scripts\hello.py)");

    // Bind event callbacks
    CameraSystem cameraSystem;
    EventManager& em = EventManager::Get();
    em.Bind<UpdateEvent, &CameraSystem::OnUpdate>(cameraSystem);
    em.Bind<MouseEvent, &CameraSystem::OnMouseEvent>(cameraSystem);

    // Run the main render loop
    engine.Run();

    return 0;
}
