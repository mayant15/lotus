#pragma once

#include <lotus/lotus_export.h>
#include <lotus/internal/nlohmann/json.hpp>
#include <lotus/internal/entt/entt.hpp>

#include <unordered_map>
#include <functional>

#define GENERATED_BODY(COMPONENT, ...) \
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(COMPONENT, __VA_ARGS__); \
    static std::string GetName() { return QUOTE(COMPONENT); }

using component_ctor_key_t = std::string;
using component_ctor_t = std::function<void(const entt::entity, entt::registry&, const nlohmann::json& data)>;

namespace Lotus
{
    extern LOTUS_API std::unordered_map<component_ctor_key_t, component_ctor_t> ctors;

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
    inline void RegisterComponent()
    {
        ctors.insert({T::GetName(), &ComponentAssigner<T>::Assign});
    }

    inline const component_ctor_t& GetComponentCtor(const std::string& name)
    {
        return ctors.at(name);
    }
}
