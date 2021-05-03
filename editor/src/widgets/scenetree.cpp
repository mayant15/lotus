#include <widgets.h>
#include <lotus/scene/SceneManager.h>
#include <lotus/ecs/ComponentRegistry.h>

namespace Editor::Widgets
{
    void SceneTree()
    {
        static bool open = true;
        ImGui::Begin("Scene", &open);

        auto scene = Lotus::SceneManager::GetCurrentScene();
        auto* reg = scene->GetRegistry();

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Root"))
        {
            auto view = reg->view<Lotus::CDisplayName>();
            for (const auto e : view)
            {
                auto& [name] = view.get(e);
                if (ImGui::TreeNode(name.c_str()))
                {
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        ImGui::End();
    }
}
