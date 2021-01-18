#include "utils.h"
#include "events.h"

#include <nfd.h>

#include <lotus/ecs/Entity.h>
#include <lotus/ecs/EventManager.h>
#include <lotus/scene/SceneManager.h>

namespace Editor
{
    std::optional<std::string> OpenFileDialog()
    {
        nfdchar_t *outPath = nullptr;
        nfdresult_t result = NFD_OpenDialog( nullptr, nullptr, &outPath );

        if ( result == NFD_OKAY ) {
            std::string path (outPath);
            free(outPath);

            return { path };
        }
        else
        {
            return std::nullopt;
        }
    }

    std::string ExpandPath(const std::string& path)
    {
        return EDITOR_RESOURCE_ROOT + path;
    }

    void LoadScene(const std::string& path)
    {
        using namespace Lotus;
        SceneManager::LoadScene(path);
        auto& em = GET(EventManager);
        em.Dispatch(Editor::SceneLoadEvent {});
        em.Dispatch(Lotus::BeginEvent {});
    }
}
