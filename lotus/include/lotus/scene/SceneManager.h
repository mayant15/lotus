#pragma once

#include "lotus/lcommon.h"
#include "lotus/scene/Scene.h"

namespace Lotus
{
    class SceneManager : public Singleton<SceneManager>
    {
        URef<Scene> _activeScene;

    public:
        const Scene& LoadScene(const std::string& path);
        void RemoveScene();

        [[nodiscard]] const Scene& GetCurrentScene() const;

    private:
        SceneManager() = default;
        friend Singleton<SceneManager>;
    };
}