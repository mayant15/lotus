#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "lotus/events/Window.h"

namespace Lotus
{
    class GLWindow : public Window
    {
        GLFWwindow* _pWindow;
        std::function<void(Event&)> _dispatchEvent;

    public:
        GLWindow(const WindowOp& options);

        bool IsVSync() const override;

        uint32_t GetWidth() const override;

        uint32_t GetHeight() const override;

        void* GetNativeWindow() const override;

        void SetEventCallback(const std::function<void(Event&)>& callback) override;

        void SetVSync(bool enabled) override;

        void OnPostUpdate() override;

        void OnDestroy() override;

        void OnShutdown() override;
    };
}
