/**
 * Defines constructs used throughout the project.
 */

#pragma once

#include <memory>

#include "lotus_export.h"
#include "lotus/internal/entt/entt.hpp"
#include "lotus/internal/cereal/cereal.hpp"
#include "lotus/internal/cereal/archives/json.hpp"


// TODO: Convenient, but not very readable I guess.
#define LOADER(x, y) struct LOTUS_API x final: entt::resource_loader<x, y>

namespace Lotus
{
    /**
     * Alias to a unique pointer.
     */
    template<typename T>
    using URef = std::unique_ptr<T>;

    /**
     * Alias to a shared pointer.
     */
    template<typename T>
    using SRef = std::shared_ptr<T>;

    template<typename T>
    using Handle = entt::resource_handle<T>;

    /**
     * Supported Render APIs.
     */
    enum class ERenderAPI
    {
        OPEN_GL,
        DIRECTX,
        VULKAN
    };

    /**
     * Base class that implements singleton behavior with CRTP.
     * @tparam T Class that is to be defined as a singleton
     */
    template<typename T>
    class LOTUS_API Singleton
    {
    public:
        Singleton(const Singleton&) = delete;

        Singleton& operator=(const Singleton&) = delete;

        /**
         * Get an instance to the class, creating it if none exist.
         * @return A reference to the active instance
         */
        static T& Get()
        {
            static T instance;  // NOLINT(clang-diagnostic-exit-time-destructors)
            return instance;
        }

    protected:
        Singleton() noexcept = default;

        virtual ~Singleton() = default;
    };
}
