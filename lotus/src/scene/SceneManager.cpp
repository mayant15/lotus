#include <lotus/scene/SceneManager.h>

#include <lotus/ecs/ComponentRegistry.h>
#include <lotus/ecs/Entity.h>
#include <lotus/resources/AssetRegistry.h>
#include <lotus/debug.h>

#include <fstream>

namespace Lotus::SceneManager
{
    inline void attachComponents(Entity entity, const nlohmann::json& info)
    {
        auto reg = GetRegistry();
        auto id = (EntityID) entity;
        for (auto& comp : info.items())
        {
            if (comp.key() != "Recipe")
            {
                auto ct = GET_COMPONENT_CTOR (comp.key());
                ct (id, *reg, comp.value());
            }
        }
    }

    void LoadScene(const std::string& path)
    {
        std::ifstream infile (path);
        nlohmann::json data;
        infile >> data;

        if (!data.is_array())
        {
            LOG_ERROR("Invalid scene format");
            throw std::exception("invalid scene format");
        }

        for (auto& entityInfo : data)
        {
            if (entityInfo.contains("Recipe"))
            {
                auto entity = CreateEntity(RESOURCE(entityInfo.at("Recipe").get<std::string>()));
                attachComponents(entity, entityInfo);
            }
            else
            {
                auto entity = CreateEntity();
                attachComponents(entity, entityInfo);
            }
        }

        // TODO: Generate the scene tree
    }
}
