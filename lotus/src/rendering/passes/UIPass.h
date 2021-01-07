#pragma once

#include <rendering/RenderPass.h>
#include <core/Engine.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Lotus::Renderer
{
    class UIPass final : public RenderPass
    {
    public:
        UIPass()
        {
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            // Setup Dear ImGui style
            ImGui::StyleColorsLight();

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) Lotus::Engine::GetNativeWindow(), true);
            const char* glsl_version = "#version 450 core";
            ImGui_ImplOpenGL3_Init(glsl_version);
            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        }

        void RenderFrame() override
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow(nullptr);
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        ~UIPass()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
    };
}
