#pragma once

#include "Scene.h"
#include "lotus/lcommon.h"

namespace Lotus
{
    class SceneManager : public Singleton<SceneManager>
    {
        URef<Scene> _activeScene;
    private:
        friend Singleton<SceneManager>;

        SceneManager() = default;

    public:

        const URef<Scene>& GetActiveScene();

        const URef<Scene>& LoadScene(const std::string& path);

        const URef<Scene>& ChangeScene(const std::string& path);
    };
}