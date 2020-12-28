#include <lotus/lotus.h>
#include <lotus/debug.h>
#include "CameraSystem.hpp"

void OnCollision(const Lotus::CollisionEvent& event)
{
    LOG_INFO("Contact");
}

// TODO: Eventually, the game classes should just describe behavior
//  Lotus should create a main function, driver facade
int main()
{
    using namespace Lotus;

    // TODO: Once we have an editor, this is the file that will be opened.
    //  But till then let's pass it in as a macro defined by CMake.
    LoadConfig(PROJECT_SETTINGS);
    Lotus::Initialize();

    // Load scene
    // SampleScene is the main scene for the pong game
    // MaterialDisplay showcases the renderer for a wide range of materials

//    SceneManager::LoadScene(ExpandPath("res://scenes/SampleScene.json"));
    SceneManager::LoadScene(ExpandPath("res://scenes/MaterialDisplay.json"));

    // Bind event callbacks
    // TODO: Bind with reflection
    EventManager& em = GET(EventManager);
    em.Bind<UpdateEvent, CameraSystem::OnUpdate>();
    em.Bind<MouseEvent, CameraSystem::OnMouseEvent>();
    em.Bind<CollisionEvent, OnCollision>();

    // Run the main render loop
    Lotus::Run();

    return 0;
}
