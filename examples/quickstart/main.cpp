#include "lotus/lotus.h"
#include "lotus/debug.h"

#include "CameraSystem.hpp"

int main()
{
    // Have to initialize the engine first
    Lotus::Engine& engine = Lotus::Engine::Get();
    Lotus::LotusOp config;
    config.RenderAPI = Lotus::ERenderAPI::OPEN_GL;
    config.IsDebug = true;
    config.Width = 1024;
    config.Height = 800;
    engine.Initialize(config);

    // Create the shaders to be used
    Lotus::SRef<Lotus::LShader> shader = std::make_shared<Lotus::LShader>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/standard.vsh",
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/diffuse.fsh"
    );

    Lotus::SRef<Lotus::LShader> skyShader = std::make_shared<Lotus::LShader>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/skybox.vsh",
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/skybox.fsh"
    );

    // Initialize and import the model
    Lotus::SRef<Lotus::LModel> model = std::make_shared<Lotus::LModel>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/mesh/untitled.obj"
    );
    model->import();

    Lotus::SRef<Lotus::LModel> planeModel = std::make_shared<Lotus::LModel>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/mesh/plane.obj"
    );
    planeModel->import();

    std::vector<std::string> faces
            {
                    "/home/priyansh/code/lotus/examples/quickstart/resources/skybox/right.jpg",
                    "/home/priyansh/code/lotus/examples/quickstart/resources/skybox/left.jpg",
                    "/home/priyansh/code/lotus/examples/quickstart/resources/skybox/top.jpg",
                    "/home/priyansh/code/lotus/examples/quickstart/resources/skybox/bottom.jpg",
                    "/home/priyansh/code/lotus/examples/quickstart/resources/skybox/front.jpg",
                    "/home/priyansh/code/lotus/examples/quickstart/resources/skybox/back.jpg",
            };
    Lotus::SRef<Lotus::Cubemap> cubemap = std::make_shared<Lotus::Cubemap>(faces);
    cubemap->import();

    Lotus::SceneManager& sceneManager = Lotus::SceneManager::Get();
    const Lotus::URef<Lotus::Scene>& scene = sceneManager.LoadScene("");

    // Sky
    Lotus::Entity sky = scene->CreateEntity();
    Lotus::CSkybox cSkybox{
            .Shader = skyShader,
            .Map = cubemap,
    };
    sky.AddComponent<Lotus::CSkybox>(cSkybox);

    // Box
    Lotus::AActor entity = scene->CreateActor(ORIGIN);
    Lotus::CMeshRenderer meshRenderer;
    meshRenderer.Shader = shader;
    meshRenderer.Model = model;
    entity.AddComponent<Lotus::CMeshRenderer>(meshRenderer);

    // Plane
    Lotus::AActor plane = scene->CreateActor(-2.0f * Y_AXIS);
    Lotus::CMeshRenderer planeRenderer;
    planeRenderer.Shader = shader;
    planeRenderer.Model = planeModel;
    plane.AddComponent<Lotus::CMeshRenderer>(planeRenderer);

    // Directional light
    Lotus::AActor dirLight = scene->CreateActor(5.0f * X_AXIS);
    Lotus::CDirectionalLight cDirectionalLight;
    cDirectionalLight.direction = Vector3f(-1.0f, -1.0f, 1.0f);
    dirLight.AddComponent<Lotus::CDirectionalLight>(cDirectionalLight);

    // Spotlight
    Lotus::AActor spotlight = scene->CreateActor(5.0f * Y_AXIS);
    Lotus::CSpotlight cSpotlight;
    cSpotlight.position = 5.0f * Y_AXIS;
    cSpotlight.direction = -1.0f * Y_AXIS;
    spotlight.AddComponent<Lotus::CSpotlight>(cSpotlight);

    // Point light
    Lotus::AActor pointLight = scene->CreateActor(3.0f * Y_AXIS - 2.0f * X_AXIS);
    Lotus::CPointLight cPointLight;
    cPointLight.position = 3.0f * Y_AXIS - 2.0f * X_AXIS;
    pointLight.AddComponent<Lotus::CPointLight>(cPointLight);

    // Camera
    Lotus::ACamera camera = scene->CreateCamera(10.0f * Z_AXIS + 5.0f * Y_AXIS, 45, true);
    camera.GetTransform().Rotation = Vector3f(-20, -90, 0);

    // Test out python script
    exec_file("/home/priyansh/code/lotus/examples/quickstart/resources/scripts/hello.py");

    // Bind event callbacks
    CameraSystem cameraSystem;
    Lotus::EventManager& em = Lotus::EventManager::Get();
    em.Bind<UpdateEvent, &CameraSystem::OnUpdate>(cameraSystem);
    em.Bind<MouseEvent, &CameraSystem::OnMouseEvent>(cameraSystem);

    // Run the main render loop
    engine.Run();

    return 0;
}
