#include "lotus/scene/SceneManager.h"

namespace Lotus
{
    const Scene & SceneManager::GetCurrentScene() const
    {
        return *_activeScene;
    }

    const Scene & SceneManager::LoadScene(const std::string& path)
    {
        // Read the scene file at the path
        _activeScene = std::make_unique<Scene>();
        _activeScene->Path = path;
        return *_activeScene;
    }

    void SceneManager::RemoveScene()
    {
        _activeScene.reset();
    }
}
