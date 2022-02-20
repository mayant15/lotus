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
                auto info = GetComponentInfo(comp.key());
                info.assignFn(id, *reg, comp.value());
            }
        }
    }

    void LoadScene(const std::string& relpath)
    {
        currentScene = std::make_shared<Scene>();
        currentScene->detail.path = relpath;
        auto fullpath = ExpandPath(relpath);

        std::ifstream infile (fullpath);
        nlohmann::json data;
        infile >> data;

//        if (!data.is_array())
        if (!data.contains("components"))
        {
            throw std::runtime_error { "Invalid scene format" };
        }

//        for (auto& entityInfo : data)
        for (auto& entityInfo : data["components"])
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

    void OnSimulationBegin(const SimulationBeginEvent& event)
    {
        // Save a snapshot of the scene
    }

    void OnSimulationEnd(const SimulationEndEvent& event)
    {
        // TODO: Don't read this from disk again? Keep in memory?
        auto fullpath = ExpandPath(currentScene->detail.path);

        std::ifstream infile (fullpath);
        nlohmann::json data;
        infile >> data;

        // Restore the scene with snapshot
        for (const auto& change : currentScene->changes())
        {
            for (const auto& comp : data["components"][std::to_string(static_cast<size_t>(change))].items())
            {
                auto info = GetComponentInfo(comp.key());
                info.assignFn(
                        change,
                        *currentScene->GetRegistry(),
                        comp.value()
                );
            }
        }

        // TODO: currentScene->hasChanged = false
    }

    void SaveScene()
    {
        // TODO: Save changes to disk. I'll probably need to fix the serialization thing once and for all
        // TODO: The above is done, sort of. Save prefab information too.
        LOG_INFO("Saving scene...");

        auto* reg = currentScene->GetRegistry();
        OutputArchive archive { reg };

        // Save all components
        SerializeComponents(archive);

        // Dump to file
        archive.DumpToFile(Lotus::ExpandPath(currentScene->detail.path));
        LOG_INFO("Saved!");
    }
}
