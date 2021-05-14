#include <widgets.h>
#include <utils.h>

#include <lotus/scene/SceneManager.h>

#include <string>
#include <optional>

namespace Editor::Widgets
{
    static void showMetricsWindow(float fps, bool* show)
    {
        if (ImGui::Begin("Performance Metrics", show))
        {
            ImGui::Text("FPS: %.1f", fps);
        }
        ImGui::End();
    }

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

                if (ImGui::MenuItem("Save Scene"))
                {
                    Lotus::SceneManager::SaveScene();
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

            static bool viewDemo = false;
            static bool viewMetrics = false;
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Demo Window", "")) viewDemo = true;
                if (ImGui::MenuItem("Metrics", "")) viewMetrics = true;
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();

            // Windows
            if (viewDemo) ImGui::ShowDemoWindow(&viewDemo);
            if (viewMetrics) showMetricsWindow(fps, &viewMetrics);
        }
    }
}
