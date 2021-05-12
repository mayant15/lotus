#pragma once

#include <lotus/lcommon.h>
#include <lotus/serialization.h>

#include <string>
#include <unordered_map>
#include <functional>

#define GENERATED_BODY(COMPONENT, ...) \
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(COMPONENT, __VA_ARGS__); \
    static std::string GetName() { return QUOTE(COMPONENT); }

namespace Lotus
{
    struct ComponentInfo;
    extern LOTUS_API std::unordered_map<entt::id_type, ComponentInfo> component_info;
    extern LOTUS_API std::unordered_map<std::string, entt::id_type> str_to_id;

    struct ComponentInfo
    {
        using AssignFn = std::function<void(const entt::entity, entt::registry&, const nlohmann::json& data)>;
        using DefaultAssignFn = std::function<void(const entt::entity, entt::registry&)>;
        using SerializeFn = std::function<void(Lotus::OutputArchive&)>;

        entt::id_type id;
        std::string name;
        AssignFn assignFn;
        DefaultAssignFn defaultAssignFn;
        SerializeFn serializeFn;
    };

    template<class Component>
    struct ComponentTraits
    {
        static void Assign(const entt::entity entity, entt::registry& registry, const nlohmann::json & data)
        {
            Component tmp;
            from_json(data, tmp);
            registry.template emplace_or_replace<Component>(entity, std::move(tmp));
        }

        static void DefaultAssign(const entt::entity entity, entt::registry& registry)
        {
            Component tmp;
            registry.template emplace_or_replace<Component>(entity, std::move(tmp));
        }

        static void Serialize(OutputArchive& archive)
        {
            archive.template components<Component>();
        }
    };

    template<class T>
    inline void RegisterComponent()
    {
        ComponentInfo meta {};
        meta.id = entt::type_info<T>::id();
        meta.name = T::GetName();
        meta.assignFn = &ComponentTraits<T>::Assign;
        meta.defaultAssignFn = &ComponentTraits<T>::DefaultAssign;
        meta.serializeFn = &ComponentTraits<T>::Serialize;

        component_info.insert({ meta.id, meta });
        str_to_id.insert({ meta.name, meta.id });
    }

    inline const ComponentInfo& GetComponentInfo(const std::string& name)
    {
        return component_info.at(str_to_id.at(name));
    }

    inline std::vector<std::string> GetRegisteredComponents()
    {
        std::vector<std::string> result;
        result.reserve(str_to_id.size());
        for (const auto& kv : str_to_id)
        {
            result.push_back(kv.first);
        }
        return result;
    }

    void RegisterEngineComponents();

    inline void SerializeComponentByID(const entt::id_type id, OutputArchive& archive)
    {
        if (component_info.find(id) != component_info.end())
        {
            component_info.at(id).serializeFn(archive);
        }
    }

    inline void SerializeComponents(OutputArchive& archive)
    {
        for (const auto& info : component_info)
        {
            info.second.serializeFn(archive);
        }
    }

    struct CDisplayName
    {
        std::string Name;
        GENERATED_BODY(CDisplayName, Name);
    };
}
