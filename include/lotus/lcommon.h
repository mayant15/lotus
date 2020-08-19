/**
 * Defines constructs used throughout the project.
 */

#pragma once

#include <memory>

#include "lotus_export.h"

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

    /**
     * Base class to define lifecycle hooks
     */
    class ILifecycle
    {
    public:
        virtual ~ILifecycle() = default;

        [[maybe_unused]] virtual void OnInit()
        {}

        [[maybe_unused]] virtual void OnBegin()
        {}

        [[maybe_unused]] virtual void OnPreUpdate()
        {}

        [[maybe_unused]] virtual void OnUpdate(float delta)
        {}

        [[maybe_unused]] virtual void OnPostUpdate()
        {}

        [[maybe_unused]] virtual void OnPreDestroy()
        {}

        [[maybe_unused]] virtual void OnDestroy()
        {}

        [[maybe_unused]] virtual void OnShutdown()
        {}
    };
}
