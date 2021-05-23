#include "properties.hpp"

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
            // Create a new entity
            if (ImGui::BeginPopupContextItem("entity-create-menu"))
            {
                if (ImGui::Selectable("Empty"))
                {
                    selected = (entt::entity) scene->CreateEntity();
                }

                // Create more entity types here

                ImGui::EndPopup();
            }

            // List existing entities
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

            // TODO: Eventually this should something like this
//            reg->visit(selected, [reg](const entt::id_type id){
//                RenderPropertyUI(reg, selected, id);
//            });

            displayComponent<Lotus::CDisplayName>(reg);
            displayComponent<Lotus::CTransform>(reg);
            displayComponent<Lotus::CSunLight>(reg);
            displayComponent<Lotus::CMeshRenderer>(reg);

            // Physics
            displayComponent<Lotus::CRigidBody>(reg);
            displayComponent<Lotus::CBoxCollider>(reg);
            displayComponent<Lotus::CSphereCollider>(reg);
            displayComponent<Lotus::CCapsuleCollider>(reg);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Add a new component
            if (ImGui::Button("Add Component"))
            {
                ImGui::OpenPopup("add_component_popup");
            }

            if (ImGui::BeginPopup("add_component_popup"))
            {
                auto components = Lotus::GetRegisteredComponents();
                for (const auto& comp : components)
                {
                    if (ImGui::Selectable(comp.c_str()))
                    {
                        auto info = Lotus::GetComponentInfo(comp);
                        info.defaultAssignFn(selected, *reg);
                    }
                }

                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }
}
