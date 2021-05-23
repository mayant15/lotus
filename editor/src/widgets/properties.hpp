/**
 * @file properties.hpp
 * @warning This file is only meant to be included by scenetree.cpp
 *
 * Helper functions and implementation for the component properties UI panel.
 */

#pragma once

#include <widgets.h>
#include <lotus/scene/SceneManager.h>
#include <lotus/ecs/ComponentRegistry.h>
#include <lotus/ecs/components/CTransform.h>
#include <lotus/ecs/components/CLight.h>
#include <lotus/rendering/CMeshRenderer.h>
#include <lotus/physics/components.h>

/**
 * @brief Helper macro to conveniently get a uniform layout for all components
 */
#define LOTUS_DISPLAY_PROPERTY_START(TITLE) \
    if (ImGui::CollapsingHeader(TITLE, ImGuiTreeNodeFlags_DefaultOpen))

namespace Editor::Widgets
{
    static entt::entity selected = entt::null;

    /**
     * Note: Component specific display functions were template specializations earlier. They have been changed
     * to be overloads instead. This will through a compiler error if we try to display a component with no defined
     * display function, which is a good thing in my opinion.
     */

    /**
     * @brief Implementation for the transform component UI
     * @param transform
     */
    void displayComponentImpl(Lotus::CTransform& transform)
    {
        float pos[3] = { transform.Position.x, transform.Position.y, transform.Position.z };
        float rot[3] = { transform.Rotation.x, transform.Rotation.y, transform.Rotation.z };
        float scale[3] = { transform.Scale.x, transform.Scale.y, transform.Scale.z };

        LOTUS_DISPLAY_PROPERTY_START("Transform")
        {
            ImGui::DragFloat3("Position", pos, 0.5f);
            ImGui::DragFloat3("Rotation", rot, 0.5f);
            ImGui::DragFloat3("Scale", scale, 0.5f);
        }

        transform.Position = { pos[0], pos[1], pos[2] };
        transform.Rotation = { rot[0], rot[1], rot[2] };
        transform.Scale = { scale[0], scale[1], scale[2] };
    }

    void displayComponentImpl(Lotus::CSunLight& sun)
    {
        ImVec4 color { sun.color.x, sun.color.y, sun.color.z, sun.color.w };
        float dir[3] = { sun.direction.x, sun.direction.y, sun.direction.z };

        LOTUS_DISPLAY_PROPERTY_START("Sunlight")
        {
            ImGui::ColorEdit4("Color", (float*) &color);
            ImGui::DragFloat3("Direction", dir, 0.5f);
        }

        sun.color = { color.x, color.y, color.z, color.w };
        sun.direction = { dir[0], dir[1], dir[2] };
    }

    void displayComponentImpl(Lotus::CMeshRenderer& mr)
    {
        // Setup buffers
        constexpr size_t MAX_PATH_LENGTH = 100;

        auto model = mr.MeshModel->detail.path;
        char modelbuf[MAX_PATH_LENGTH];
        std::memcpy(modelbuf, model.c_str(), model.length() + 1); // NOTE: Plus 1 to get the terminating \0

        auto mat = mr.MeshMaterial->detail.path;
        char matbuf[MAX_PATH_LENGTH];
        std::memcpy(matbuf, mat.c_str(), mat.length() + 1);

        // Start UI
        LOTUS_DISPLAY_PROPERTY_START("Mesh Renderer")
        {
            ImGui::InputText("Model", modelbuf, MAX_PATH_LENGTH, ImGuiInputTextFlags_ReadOnly);
            ImGui::InputText("Material", matbuf, MAX_PATH_LENGTH, ImGuiInputTextFlags_ReadOnly);
        }

        // TODO: Load the new model/material when value is changed here
    }

    void displayComponentImpl(Lotus::CDisplayName& dn)
    {
        constexpr size_t MAX_DISPLAY_NAME_SIZE = 100;
        char namebuf[100];
        std::memcpy(namebuf, dn.Name.c_str(), dn.Name.length() + 1);

        LOTUS_DISPLAY_PROPERTY_START("Display Name")
        {
            ImGui::InputText("Name", namebuf, MAX_DISPLAY_NAME_SIZE);
        }

        dn.Name = namebuf;
    }

    void displayComponentImpl(Lotus::CRigidBody& rb)
    {
        LOTUS_DISPLAY_PROPERTY_START("Rigidbody")
        {
            ImGui::InputFloat("Gravity", &rb.Gravity, 0.5f);
            ImGui::Checkbox("Is Kinematic", &rb.IsKinematic);
        }
    }

    void displayComponentImpl(Lotus::CBoxCollider& bc)
    {
        float pos[3] = { bc.Position.x, bc.Position.y, bc.Position.z };
        float dim[3] = { bc.Dimensions.x, bc.Dimensions.y, bc.Dimensions.z };

        LOTUS_DISPLAY_PROPERTY_START("Box Collider")
        {
            ImGui::InputFloat3("Position", pos);
            ImGui::InputFloat3("Dimensions", dim);
        }

        bc.Position = { pos[0], pos[1], pos[2] };
        bc.Dimensions = { dim[0], dim[1], dim[2] };
    }

    void displayComponentImpl(Lotus::CCapsuleCollider& cc)
    {
        float pos[3] = { cc.Position.x, cc.Position.y, cc.Position.z };
        LOTUS_DISPLAY_PROPERTY_START("Capsule Collider")
        {
            ImGui::InputFloat3("Position", pos);
            ImGui::InputFloat("Height", &cc.Height);
            ImGui::InputFloat("Radius", &cc.Radius);
        }
        cc.Position = { pos[0], pos[1], pos[2] };
    }

    void displayComponentImpl(Lotus::CSphereCollider& sc)
    {
        float pos[3] = { sc.Position.x, sc.Position.y, sc.Position.z };
        LOTUS_DISPLAY_PROPERTY_START("Sphere Collider")
        {
            ImGui::InputFloat3("Position", pos);
            ImGui::InputFloat("Radius", &sc.Radius);
        }
        sc.Position = { pos[0], pos[1], pos[2] };
    }

    template<class Component>
    void displayComponent(entt::registry* reg)
    {
        if (reg->template has<Component>(selected))
        {
            auto& comp = reg->template get<Component>(selected);
            displayComponentImpl(comp);
        }
    }
}
