#include "widgets.h"
#include "utils.h"
#include "EditorCamera.h"

#include <lotus/lotus.h>

#include <chrono>

/**
 * TODO: Handle engine vs runtime graphics API
 *   One way to do this would be to always run the engine and the game in OpenGL, but package with other APIs.
 *   Another thing we could do is have the editor run in OpenGL, but the "play" button will launch a new window with the game.
 *   Finally, we could run the editor and the game with the API mentioned in settings.
 */
int main(int argc, const char** argv)
{
    using namespace Editor;

    // NOTE: Setting up the window context and loading the graphics API is the responsibility
    // of the client. The editor does this here, but when finally packaging the game we need to insert
    // a stub main function
    Window* window = CreateNewWindow();

    Lotus::LoadConfig(std::string {argv[1]});
    Lotus::Engine::Initialize();

    // Renderer has been set up, setup ImGui panels
    Editor::Widgets::Initialize(window);

    // Keep an empty scene always loaded
    Editor::LoadScene(Lotus::ExpandPath("res://scenes/MaterialDisplay.json"));

    // TODO: Bind somewhere else
    GET(Lotus::EventManager).Bind<Editor::SceneLoadEvent, Editor::OnSceneLoad>();

    auto currentTime = std::chrono::system_clock::now();
    auto lastTime = currentTime;
    while (!ShouldCloseWindow(window))
    {
        // tick
        currentTime = std::chrono::system_clock::now();
        std::chrono::duration<double> delta = currentTime - lastTime;
        double deltaTime = delta.count();
        lastTime = currentTime;

        StartFrame(window);
        Widgets::StartFrame(deltaTime);

        Lotus::Engine::Tick(deltaTime);

        Widgets::EndFrame(deltaTime);
        EndFrame(window);
    }

    Editor::Widgets::Shutdown();
    Lotus::Engine::Shutdown();
    DestroyWindow(window);

    return 0;
}
