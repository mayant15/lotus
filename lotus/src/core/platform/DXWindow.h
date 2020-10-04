#pragma once

#include "core/IWindow.h"

namespace Lotus
{
    class DXWindow final : public IWindow
    {
//        GLFWwindow* _pWindow;
        std::function<void(Event&)> _dispatchEvent;

    public:
        DXWindow(const WindowOp& options);

        [[nodiscard]] bool IsVSync() const override;

        [[nodiscard]] uint32_t GetWidth() const override;

        [[nodiscard]] uint32_t GetHeight() const override;

        [[nodiscard]] void* GetNativeWindow() const override;

        void SetEventCallback(const std::function<void(Event&)>& callback) override;

        void SetVSync(bool enabled) override;

        void OnPostUpdate(const PostUpdateEvent& event) override;
        void OnDestroy(const DestroyEvent& event) override;
        void OnShutdown(const ShutdownEvent& event) override;
    };
}
