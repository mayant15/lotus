#include "lotus/scene/SceneManager.h"

namespace Lotus
{
    const URef <Scene>& SceneManager::GetActiveScene()
    {
        return _activeScene;
    }

    const URef <Scene>& SceneManager::LoadScene(const std::string& path)
    {
        // TODO: Load from path
        _activeScene = std::make_unique<Scene>();
        return _activeScene;
    }

    const URef <Scene>& SceneManager::ChangeScene(const std::string& path)
    {
        // TODO: Destroy current scene?
        return LoadScene(path);
    }
}