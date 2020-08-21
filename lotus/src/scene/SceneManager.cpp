#include "lotus/scene/SceneManager.h"
#include "lotus/scene/ACamera.h"

#include <fstream>

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
        _activeScene->Path = path;
        return _activeScene;
    }

    const URef <Scene>& SceneManager::ChangeScene(const std::string& path)
    {
        // TODO: Destroy current scene?
        return LoadScene(path);
    }

    void SceneManager::OnPreDestroy(const PreDestroyEvent& event)
    {
        std::ofstream sceneFile(_activeScene->Path);
        OArchive archive(sceneFile);
        _activeScene->Save(archive);
    }

    ACamera SceneManager::GetActiveCamera()
    {
        return _activeScene->GetActiveCamera();
    }
}
