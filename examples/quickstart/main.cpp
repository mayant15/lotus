#include "lotus/lotus.h"
#include "CameraSystem.hpp"

// Include the scene that you want to render
#include "PBR.hpp"

int main()
{
    // Have to initialize the engine first
    auto& config = GET(Config);
    config.RenderAPI = ERenderAPI::OPEN_GL;
    config.IsDebug = true;
    config.Width = 1024;
    config.Height = 800;
    config.ResourceRoot = DEFAULT_RESOURCE_ROOT;

    auto& engine = GET(Engine);
    engine.Initialize();

    setup();

    // Test out python script
    exec_file(RESOURCE("scripts/hello.py"));

    // Bind event callbacks
    CameraSystem cameraSystem;
    EventManager& em = EventManager::Get();
    em.Bind<UpdateEvent, &CameraSystem::OnUpdate>(cameraSystem);
    em.Bind<MouseEvent, &CameraSystem::OnMouseEvent>(cameraSystem);

    // Run the main render loop
    engine.Run();

    return 0;
}
