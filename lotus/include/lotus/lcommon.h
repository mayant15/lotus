/**
 * Defines constructs used throughout the project.
 */

#pragma once

#include "lotus_export.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include <fstream>
#include <memory>
#include <type_traits>

#define Q(x) #x
#define QUOTE(x) Q(x)

#define SERIALIZE(Type, ...) NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Type, __VA_ARGS__)
#define SERIALIZE_ENUM(Type, ...) NLOHMANN_JSON_SERIALIZE_ENUM(Type, __VA_ARGS__)

#define GET(x) x::Get()

namespace Lotus {
/**
 * Alias to a unique pointer.
 */
template <typename T> using URef = std::unique_ptr<T>;

/**
 * Alias to a shared pointer.
 */
template <typename T> using SRef = std::shared_ptr<T>;

template <typename T> using Handle = entt::resource_handle<T>;

/**
 * Base class that implements singleton behavior with CRTP.
 * @tparam T Class that is to be defined as a singleton
 */
template <typename T> class LOTUS_API Singleton
{
  public:
    Singleton(const Singleton &) = delete;

    Singleton &operator=(const Singleton &) = delete;

    /**
     * Get an instance to the class, creating it if none exist.
     * @return A reference to the active instance
     */
    static T &Get()
    {
        static T instance;
        return instance;
    }

  protected:
    Singleton() noexcept = default;

    virtual ~Singleton() = default;
};
} // namespace Lotus
