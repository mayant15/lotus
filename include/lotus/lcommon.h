/**
 * Defines constructs used throughout the project.
 */

#pragma once

#include <memory>

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
     * @example
     * <code>
     *     class Sample: public Singleton<Sample>
     *     {
     *     private:
     *         friend Singleton<Sample>; // Required to access the private constructor
     *         Sample() = default;
     *     public:
     *         // Public member functions
     *     }
     *</code>
     * @tparam T Class that is to be defined as a singleton
     */
    template<typename T>
    class Singleton
    {
    public:
        Singleton(const Singleton&) = delete;

        Singleton& operator=(const Singleton&) = delete;

        /**
         * Get an instance to the class, creating it if none exist.
         * @return A reference to the active instance
         */
        static T& Get() noexcept(std::is_nothrow_constructible<T>::value)
        {
            static T instance;
            return instance;
        }

    protected:
        Singleton() noexcept = default;

        virtual ~Singleton() = default;
    };

    /**
     * Base class to define lifecyle hooks
     */
    class ILifecycle
    {
    public:
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
