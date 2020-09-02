#include "lotus/scene/SceneManager.h"
#include "lotus/scene/ACamera.h"

#include <fstream>

#include "lotus/debug.h"

namespace Lotus
{
    using json = nlohmann::json;

    const URef <Scene>& SceneManager::GetActiveScene()
    {
        return _activeScene;
    }

    const URef <Scene>& SceneManager::LoadScene(const std::string& path)
    {
        _activeScene = std::make_unique<Scene>();
        _activeScene->Path = path;

        std::ifstream file(path);
        json data;

        // TODO: Generate entities from this json


        return _activeScene;
    }

    const URef <Scene>& SceneManager::ChangeScene(const std::string& path)
    {
        // TODO: Destroy current scene?
        _activeScene.reset();
        return LoadScene(path);
    }

    void SceneManager::OnPreDestroy(const PreDestroyEvent& event)
    {
        // std::ofstream sceneFile(_activeScene->Path);
        // OArchive archive(sceneFile);
        // _activeScene->Save(archive);

        LOG_WARN("Should save the scene here...");
    }

    ACamera SceneManager::GetActiveCamera()
    {
        return _activeScene->GetActiveCamera();
    }
}
