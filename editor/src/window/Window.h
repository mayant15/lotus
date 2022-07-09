#pragma once

#include <lotus/Config.h>

namespace Editor
{
    struct WindowOp
    {
        Lotus::ERenderAPI RenderBackend = Lotus::ERenderAPI::OPEN_GL;
        bool EnableVSync = true;
    };

    class Window
    {
    public:
        const void *get_raw_pointer() const;
        bool should_close() const;

        void start_frame() const;
        void end_frame() const;

    private:
        Window(const WindowOp &options);
        ~Window();

        void *m_window_instance = nullptr;
        friend class WindowManager;
    };
}
