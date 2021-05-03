#include <widgets.h>
#include <lotus/scene/SceneManager.h>
#include <lotus/ecs/ComponentRegistry.h>
#include <lotus/ecs/components/CTransform.h>
#include <lotus/ecs/components/CLight.h>

namespace Editor::Widgets
{
    static entt::entity selected = entt::null;

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
                ImGuiTreeNodeFlags node_flags = 0;
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                if (selected == e)
                {
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                }

                auto& [name] = view.get(e);
                if (ImGui::TreeNodeEx(name.c_str(), node_flags))
                {
                    if (ImGui::IsItemClicked())
                    {
                        selected = e;
                    }
                }
            }

            ImGui::TreePop();
        }

        ImGui::End();
    }

    template<class Component>
    void properties(entt::registry* reg) {}

    template<> void properties<Lotus::CTransform>(entt::registry* reg)
    {
        if (reg->has<Lotus::CTransform>(selected))
        {
            auto& transform = reg->get<Lotus::CTransform>(selected);
            float pos[3] = { transform.Position.x, transform.Position.y, transform.Position.z };
            float rot[3] = { transform.Rotation.x, transform.Rotation.y, transform.Rotation.z };
            float scale[3] = { transform.Scale.x, transform.Scale.y, transform.Scale.z };

            ImGui::Text("TRANSFORM:");
            ImGui::DragFloat3("Position", pos, 0.5f);
            ImGui::DragFloat3("Rotation", rot, 0.5f);
            ImGui::DragFloat3("Scale", scale, 0.5f);
            ImGui::Separator();

            transform.Position = { pos[0], pos[1], pos[2] };
            transform.Rotation = { rot[0], rot[1], rot[2] };
            transform.Scale = { scale[0], scale[1], scale[2] };
        }
    }

    template<> void properties<Lotus::CSunLight>(entt::registry* reg)
    {
        using namespace Lotus;
        if (reg->has<CSunLight>(selected))
        {
            auto& sun = reg->get<CSunLight>(selected);
            ImVec4 color { sun.color.x, sun.color.y, sun.color.z, sun.color.w };
            float dir[3] = { sun.direction.x, sun.direction.y, sun.direction.z };

            ImGui::Text("SUNLIGHT:");
            ImGui::ColorEdit4("Color", (float*) &color);
            ImGui::DragFloat3("Direction", dir, 0.5f);
            ImGui::Separator();

            sun.color = { color.x, color.y, color.z, color.w };
            sun.direction = { dir[0], dir[1], dir[2] };
        }
    }

    void Properties()
    {
        static bool show = true;
        ImGui::Begin("Properties", &show);

        if (selected == entt::null)
        {
            ImGui::Text("No entity selected.");
        }
        else
        {
            auto scene = Lotus::SceneManager::GetCurrentScene();
            auto* reg = scene->GetRegistry();
            properties<Lotus::CTransform>(reg);
            properties<Lotus::CSunLight>(reg);
        }

        ImGui::End();
    }
}
