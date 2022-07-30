#include "EditorCamera.h"
#include "ModuleLoader.h"
#include "gizmos.h"
#include "utils.h"
#include "widgets.h"

#include <lotus/debug.h>
#include <lotus/lotus.h>

#include <chrono>
#include <exception>


/**
 * TODO: Handle engine vs runtime graphics API
 *   One way to do this would be to always run the engine and the game in OpenGL, but package with other APIs.
 *   Another thing we could do is have the editor run in OpenGL, but the "play" button will launch a new window with the
 * game. Finally, we could run the editor and the game with the API mentioned in settings.
 */
int safe_main(int argc, const char **argv)
{
    // Validate input
    if (argc < 2) {
        LOG_ERROR("Please provide Project Directory.");
        return EXIT_FAILURE;
    }

    std::filesystem::path projectRoot{argv[1]};

    if (!projectRoot.is_absolute() || projectRoot.has_filename()) {
        LOG_ERROR("Invalid project directory provided: {}", argv[1]);
        return EXIT_FAILURE;
    }

    using namespace Editor;

    // NOTE: Setting up the window context and loading the graphics API is the responsibility
    // of the client. The editor does this here, but when finally packaging the game we need to insert
    // a stub main function
    Window *window = CreateNewWindow();

    Lotus::LoadConfig(projectRoot);
    Lotus::Engine::Initialize();

    // Renderer has been set up, setup ImGui panels
    Editor::Widgets::Initialize(window);
    Editor::Gizmos::Initialize(window);

    auto conf = Lotus::GetProjectConfig();
    LoadModule("quickstart.dll");

    // TODO: Keep an empty scene always loaded
    auto &em = Lotus::EventManager::Get();
    em.Bind<Lotus::SceneLoadEvent, Editor::SetupEditorCamera>();

    LOG_INFO("Loading scene...");
    Lotus::SceneManager::LoadScene(conf.StartScene);
    LOG_INFO("Loaded!");

    LOG_INFO("Starting main loop...");

    auto currentTime = std::chrono::system_clock::now();
    auto lastTime = currentTime;
    while (!ShouldCloseWindow(window)) {
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

    LOG_INFO("Shutting down...");

    Editor::Gizmos::Shutdown();
    Editor::Widgets::Shutdown();
    Lotus::Engine::Shutdown();
    DestroyWindow(window);

    return 0;
}

int main(int argc, const char **argv)
{
    try {
        if (safe_main(argc, argv) == 0) {
            // All good!
            return 0;
        } else {
            throw std::runtime_error("There was a problem");
        }
    } catch (const std::exception &e) {
        LOG_ERROR("{}", e.what());
        return EXIT_FAILURE;
    }
}
