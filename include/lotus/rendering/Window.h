#pragma once

#include <functional>
#include <utility>

namespace Lotus
{
    struct WindowOp
    {
        std::string Title = "Lotus";
        uint32_t Width = 1024;
        uint32_t Height = 720;
    };

    class Event;

    // Thank you Cherno :)
    class Window
    {
    protected:
        WindowOp _options;
    public:
        Window(WindowOp options) : _options(std::move(options)) {}

        virtual void OnUpdate(float delta) = 0;

        virtual bool IsVSync() const = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual void* GetNativeWindow() const = 0;

        virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
    };
}
