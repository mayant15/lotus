#pragma once

#include <memory>

namespace Lotus
{
    // Aliases
    template<typename T>
    using URef = std::unique_ptr<T>;

    template<typename T>
    using SRef = std::shared_ptr<T>;

    enum class ERenderAPI
    {
        OPEN_GL,
        DIRECTX,
        VULKAN
    };

    template<typename T>
    class Singleton
    {
    protected:
        Singleton() noexcept = default;

        virtual ~Singleton() = default;

    public:
        Singleton(const Singleton&) = delete;

        Singleton& operator=(const Singleton&) = delete;

        static T& Get() noexcept(std::is_nothrow_constructible<T>::value)
        {
            static T instance;
            return instance;
        }
    };

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
