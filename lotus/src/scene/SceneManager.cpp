#include <lotus/scene/SceneManager.h>
#include <lotus/ecs/ComponentRegistry.h>
#include <lotus/debug.h>

#include <lotus/filesystem.h>

#include <stdexcept>
#include <lotus/ecs/EventManager.h>

namespace Lotus::SceneManager
{
    SRef<Scene> currentScene;

    SRef<Scene> GetCurrentScene()
    {
        return currentScene;
    }

    inline void attachComponents(Entity entity, const nlohmann::json& info)
    {
        auto reg = currentScene->GetRegistry();
        auto id = (EntityID) entity;

        // TODO: Transform should be created first. Right now, component create events are non-immediate, so they're
        //  called only after all components are attached. While that works for now, we should really have some sort of
        //  strict order here.

//        if (info.contains("CTransform"))
//        {
//            // Add a transform first as other components might depend on it
//            auto entity = CreateEntity();
//            CTransform transform;
//            from_json(entityInfo.at("CTransform"), transform);
//            entity.AddComponent<CTransform>(transform);
//        }

        if (!info.contains("CDisplayName"))
        {
            entity.AddComponent<CDisplayName>("Entity " + std::to_string((unsigned int) id));
        }

        for (auto& comp : info.items())
        {
            if (comp.key() != "Prefab")
            {
                auto ct = GetComponentCtor(comp.key());
                ct (id, *reg, comp.value());
            }
        }
    }

    void LoadScene(const std::string& path)
    {
        currentScene = std::make_shared<Scene>();

        std::ifstream infile (path);
        nlohmann::json data;
        infile >> data;

        if (!data.is_array())
        {
            throw std::runtime_error { "Invalid scene format" };
        }

        for (auto& entityInfo : data)
        {
            if (entityInfo.contains("Prefab"))
            {
                auto entity = currentScene->CreateEntity(ExpandPath(entityInfo.at("Prefab").get<std::string>()));
                attachComponents(entity, entityInfo);
            }
            else
            {
                auto entity = currentScene->CreateEntity();
                attachComponents(entity, entityInfo);
            }
        }

        // TODO: Generate the scene tree

        EventManager::Get().Dispatch(SceneLoadEvent { currentScene.get() });
    }
}
