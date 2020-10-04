#pragma once

#include "lotus/lcommon.h"
#include "lotus/Config.h"
#include "lotus/ILifecycle.h"
#include "lotus/ecs/Event.h"

#include <functional>
#include <utility>

namespace Lotus
{
    enum class EWindowBackend
    {
        GLFW,
        DIRECTX
    };

    /**
     * Options to configure windows
     */
    struct WindowOp
    {
        /**
         * Title of the window
         */
        std::string Title = "Lotus";

        /**
         * Width of the window
         */
        uint32_t Width = 1024;

        /**
         * Height of the window
         */
        uint32_t Height = 720;

        /**
         * Is this a debug context
         */
        bool IsDebug = true;

        EWindowBackend Backend = EWindowBackend::GLFW;

        WindowOp() = default;
        explicit WindowOp(const Config& config);
    };

    /**
     * Abstract window class to be implemented by platform specific window implementations.
     */
    class IWindow : public ILifecycle
    {
    protected:
        WindowOp _options;
    public:
        explicit IWindow(WindowOp options) : _options(std::move(options))
        {}

        virtual void SetVSync(bool enabled) = 0;

        [[nodiscard]] virtual bool IsVSync() const = 0;
        [[nodiscard]] virtual uint32_t GetWidth() const = 0;
        [[nodiscard]] virtual uint32_t GetHeight() const = 0;
        [[nodiscard]] virtual void* GetNativeWindow() const = 0;

        virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;

        void OnPostUpdate(const PostUpdateEvent& event) override = 0;
        void OnDestroy(const DestroyEvent& event) override = 0;
        void OnShutdown(const ShutdownEvent& event) override = 0;
    };
}
