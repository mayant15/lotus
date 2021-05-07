#pragma once

#include <lotus/lcommon.h>
#include <lotus/serialization.h>

#include <string>
#include <unordered_map>
#include <functional>

#define GENERATED_BODY(COMPONENT, ...) \
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(COMPONENT, __VA_ARGS__); \
    static std::string GetName() { return QUOTE(COMPONENT); }

using component_ctor_key_t = std::string;
using component_ctor_t = std::function<void(const entt::entity, entt::registry&, const nlohmann::json& data)>;
using component_serializer_t = std::function<void(Lotus::OutputArchive&)>;

namespace Lotus
{
    extern LOTUS_API std::unordered_map<component_ctor_key_t, component_ctor_t> ctors;
    extern LOTUS_API std::unordered_map<entt::id_type, component_serializer_t> serializers;

    template<class T>
    struct ComponentAssigner
    {
        static void Assign(const entt::entity entity, entt::registry& registry, const nlohmann::json & data)
        {
            T tmp;
            from_json(data, tmp);
            registry.template emplace_or_replace<T>(entity, std::move(tmp));
        }
    };

    template<class T>
    struct ComponentSerializer
    {
        static void Serialize(OutputArchive& archive)
        {
            archive.template components<T>();
        }
    };

    template<class T>
    inline void RegisterComponent()
    {
        ctors.insert({T::GetName(), &ComponentAssigner<T>::Assign});
        serializers.insert({entt::type_info<T>::id(), &ComponentSerializer<T>::Serialize});
    }

    inline const component_ctor_t& GetComponentCtor(const std::string& name)
    {
        return ctors.at(name);
    }

    void RegisterEngineComponents();

    inline void SerializeComponentByID(const entt::id_type id, OutputArchive& archive)
    {
        if (serializers.find(id) != serializers.end())
        {
            serializers.at(id)(archive);
        }
    }

    inline void SerializeComponents(OutputArchive& archive)
    {
        for (const auto& entry : serializers)
        {
            entry.second(archive);
        }
    }

    struct CDisplayName
    {
        std::string Name;
        GENERATED_BODY(CDisplayName, Name);
    };
}
