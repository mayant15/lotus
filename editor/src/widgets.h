#pragma once

#include "window/Window.h"
#include <imgui/imgui.h>

namespace Editor::Widgets
{
    void Initialize(Window* window);
    void StartFrame(double deltaTime);
    void EndFrame(double deltaTime);
    void Shutdown();

    void Viewport(unsigned int texture, float ux, float uy);
    ImVec2 GetViewportDims();
    void RegisterViewportEvents(Window* window);

    void MainDockSpace();
    void DemoWindow();
    void MainMenu(float fps);
    void Log();
}
