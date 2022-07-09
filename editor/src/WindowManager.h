#pragma once

#include <lotus/utility.h>
#include "window/Window.h"

#include <vector>

namespace Lotus::Editor
{
    class WindowManager
    {
    public:
        WindowManager(Lotus::ERenderAPI backend);
        ~WindowManager();

    private:
        std::vector<Window> m_windows {};
    };
} // namespace Lotus::Editor
