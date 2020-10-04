#include "lotus/lotus.h"
#include "CameraSystem.hpp"

// Include the scene that you want to render
#include "PBR.hpp"

// TODO: Eventually, the game classes should just describe behavior
// Lotus should create a main function, driver facade
int main()
{
    // Have to initialize the engine first
    auto& config = GET(Config);
    config.RenderAPI = ERenderAPI::OPEN_GL;
    config.IsDebug = true;
    config.Width = 1024;
    config.Height = 800;
    config.ResourceRoot = DEFAULT_RESOURCE_ROOT;

    // TODO: Move the engine class inside
    Lotus::Initialize(config);

//    auto& engine = GET(Engine);
//    engine.Initialize();

    setup();

    // Test out python script
    exec_file(RESOURCE("scripts/hello.py"));

    // Bind event callbacks
    EventManager& em = EventManager::Get();
    em.Bind<UpdateEvent, &CameraSystem::OnUpdate>();
    em.Bind<MouseEvent, &CameraSystem::OnMouseEvent>();

    // Run the main render loop
    // engine.Run();
    Lotus::Run();

    return 0;
}
