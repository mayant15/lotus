#pragma once

#include "lotus/events/Event.h"
#include "lotus/lcommon.h"

#include <functional>
#include <utility>

namespace Lotus
{
    struct WindowOp
    {
        std::string Title = "Lotus";
        uint32_t Width = 1024;
        uint32_t Height = 720;
        bool IsDebug = true;
    };

    class Window : public ILifecycle
    {
    protected:
        WindowOp _options;
    public:
        Window(WindowOp options) : _options(std::move(options))
        {}

        virtual void SetVSync(bool enabled) = 0;

        virtual bool IsVSync() const = 0;

        virtual uint32_t GetWidth() const = 0;

        virtual uint32_t GetHeight() const = 0;

        virtual void* GetNativeWindow() const = 0;

        virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;

        void OnPostUpdate() override = 0;

        void OnDestroy() override = 0;

        void OnShutdown() override = 0;
    };
}
