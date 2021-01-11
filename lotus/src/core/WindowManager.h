#pragma once

#include <string>
#include <utility>

namespace Lotus
{
    using Window = void;
}

namespace Lotus::WindowManager
{
    struct WindowInfo
    {
        unsigned int Width;
        unsigned int Height;
        std::string Title;
        bool IsDebug;
    };

    void CreateWindow();
    void StartFrame();
    void EndFrame();
    void DestroyWindow();

    void* GetWindowPointer();
    std::pair<int, int> GetDimensions();
}
