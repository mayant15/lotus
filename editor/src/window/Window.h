#pragma once

namespace Editor
{
    using Window = void;

    Window* CreateNewWindow();
    void DestroyWindow(Window* window);

    bool ShouldCloseWindow(Window* window);
    void StartFrame(Window* window);
    void EndFrame(Window* window);
}
