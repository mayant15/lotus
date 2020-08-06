#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "lotus/rendering/Window.h"

namespace Lotus
{
    class GLWindow : public Window
    {
        GLFWwindow* _pWindow;

    public:
        GLWindow(const WindowOp& options);

        void OnUpdate(float delta) override;

        bool IsVSync() const override;

        uint32_t GetWidth() const override;

        uint32_t GetHeight() const override;

        void* GetNativeWindow() const override;

        void SetEventCallback(const std::function<void(Event&)>& callback) override;

        void SetVSync(bool enabled) override;
    };
}
