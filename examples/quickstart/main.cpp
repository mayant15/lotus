#include "lotus/lotus.h"
#include "CameraSystem.hpp"

// TODO: Eventually, the game classes should just describe behavior
// Lotus should create a main function, driver facade
int main()
{
    using namespace Lotus;
    // Have to initialize the engine first
    auto& config = GET(Config);
    config.RenderAPI = ERenderAPI::OPEN_GL;
    config.IsDebug = true;
    config.Width = 1024;
    config.Height = 800;
    config.ResourceRoot = DEFAULT_RESOURCE_ROOT;

    Lotus::Initialize(config);

    SceneManager::LoadScene(RESOURCE("scenes/SampleScene.json"));

    // Test out python script
    exec_file(RESOURCE("scripts/hello.py"));

    // Bind event callbacks
    // TODO: Bind with reflection
    EventManager& em = GET(EventManager);
    em.Bind<UpdateEvent, CameraSystem::OnUpdate>();
    em.Bind<MouseEvent, CameraSystem::OnMouseEvent>();

    // Run the main render loop
    Lotus::Run();

    return 0;
}
