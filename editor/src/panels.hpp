#pragma once

#include <imgui/imgui.h>

#include "panels/mainmenu.hpp"
#include "panels/log.hpp"

/**
 * Place common ones here that don't need much logic to work
 */
namespace Editor::Panel
{
    void MainDockSpace()
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    }

    void DemoWindow()
    {
        ImGui::ShowDemoWindow();

    }
}
