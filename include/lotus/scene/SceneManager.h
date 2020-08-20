#pragma once

#include "Scene.h"
#include "lotus/lcommon.h"

namespace Lotus
{
    class LOTUS_API SceneManager : public Singleton<SceneManager>
    {
        URef<Scene> _activeScene;

        friend Singleton<SceneManager>;
    public:

        const URef<Scene>& GetActiveScene();

        ACamera GetActiveCamera();

        const URef<Scene>& LoadScene(const std::string& path);

        const URef<Scene>& ChangeScene(const std::string& path);
    };
}