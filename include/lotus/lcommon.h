#pragma once

#include <memory>

namespace Lotus
{
    // Aliases
    template<typename T>
    using URef = std::unique_ptr<T>;

    template<typename T>
    using SRef = std::shared_ptr<T>;

    template<typename T>
    using WRef = std::weak_ptr<T>;

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

        Singleton(const Singleton&) = delete;

        Singleton& operator=(const Singleton&) = delete;

        virtual ~Singleton() = default;

    public:
        static T& Get() noexcept(std::is_nothrow_constructible<T>::value)
        {
            static T instance;
            return instance;
        }
    };

    class ILifecycle
    {
        virtual void OnInit()
        {}

        virtual void OnBegin()
        {}

        virtual void OnPreUpdate()
        {}

        virtual void OnUpdate(float delta)
        {}

        virtual void OnPostUpdate()
        {}

        virtual void OnPreDestroy()
        {}

        virtual void OnDestroy()
        {}

        virtual void OnShutdown()
        {}
    };
}
