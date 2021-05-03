#include <widgets.h>
#include <utils.h>

#include <lotus/scene/SceneManager.h>

#include <string>
#include <optional>

namespace Editor::Widgets
{
    void MainMenu(float fps)
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open Scene"))
                {
                    // TODO: Do this somewhere else
                    std::optional<std::string> filepath = OpenFileDialog();
                    if (filepath.has_value())
                    {
                        Lotus::SceneManager::LoadScene(filepath.value());
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z"))
                {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
                {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X"))
                {}
                if (ImGui::MenuItem("Copy", "CTRL+C"))
                {}
                if (ImGui::MenuItem("Paste", "CTRL+V"))
                {}
                ImGui::EndMenu();
            }

            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("FPS: %.1f", fps);

            ImGui::EndMainMenuBar();
        }
    }
}
