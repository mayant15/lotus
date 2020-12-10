#pragma once

#include <lotus/internal/nlohmann/json.hpp>
#include <lotus/internal/entt/entt.hpp>

#include <unordered_map>
#include <functional>

namespace Lotus
{
    using key_t = entt::id_type;
    using data_t = nlohmann::json;
    using ctor_t = std::function<void(const entt::entity, entt::registry&, const data_t& data)>;

#define GENERATE_COMPONENT_GET_NAME_DECL() \
    static std::string GetName();

#define GENERATE_COMPONENT_GET_NAME_BODY(component) \
    inline std::string component::GetName() { \
        return QUOTE(component); \
    }

#define GENERATE_COMPONENT_ASSIGN_DECL() \
    static void assign(const entt::entity entity, entt::registry& registry, const data_t& data);

#define GENERATE_COMPONENT_ASSIGN_BODY(component) \
    inline void component::assign(const entt::entity entity, entt::registry& registry, const data_t& data) { \
        component tmp; \
        from_json(data, tmp); \
        registry.emplace_or_replace<component>(entity, std::move(tmp)); \
    }

#define GENERATE_COMPONENT_REGISTER_DECL() \
    static const ComponentRegistry registry; \

#define GENERATE_COMPONENT_REGISTER_BODY(component) \
    inline const ComponentRegistry component::registry { QUOTE(component)_hs, &component::assign };

#define REGISTER_DECL() \
public: \
    GENERATE_COMPONENT_GET_NAME_DECL() \
private: \
    GENERATE_COMPONENT_ASSIGN_DECL() \
    GENERATE_COMPONENT_REGISTER_DECL()

#define REGISTER_BODY(component, ...) \
    GENERATE_COMPONENT_GET_NAME_BODY(component) \
    GENERATE_COMPONENT_ASSIGN_BODY(component) \
    GENERATE_COMPONENT_REGISTER_BODY(component)

#define GET_COMPONENT_CTOR(name) ComponentRegistry::Get(entt::hashed_string::value(name.c_str()));

    struct ComponentRegistry
    {
        ComponentRegistry(key_t id, ctor_t func)
        {
            ctors.insert({id, func});
        }

        static const ctor_t& Get(key_t key)
        {
            return ctors.at(key);
        }

    private:
        static inline std::unordered_map<key_t, ctor_t> ctors;
    };
}
