#include "widgets.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "IconsForkAwesome.h"
#include "config.h"
#include "utils.h"
#include "window/backends/imgui_impl_glfw.h"
#include "window/backends/imgui_impl_opengl3.h"

#include <lotus/rendering/RenderPass.h>

namespace Editor::Widgets {
static unsigned int colorBuffer;
static Window *window = nullptr;

void Initialize(Window *window_)
{
    window = window_;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION_STRING);

    // Text fonts
    io.Fonts->AddFontFromFileTTF(Editor::ExpandPath("fonts/OpenSans-Regular.ttf").c_str(), 20.0f);

    // Icon fonts
    static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_FK, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF(
        Editor::ExpandPath("fonts/forkawesome-webfont.ttf").c_str(), 16.0f, &icons_config, icons_ranges);

    io.Fonts->AddFontDefault();

    colorBuffer = Lotus::Renderer::GetColorBuffer();

    RegisterViewportEvents(window);
}

void StartFrame(double deltaTime)
{
    ImVec2 dims = Editor::Widgets::GetViewportDims();
    Lotus::Renderer::SetViewport(dims.x, dims.y);
}

static void renderWidgets()
{
    // TODO: Improve render APIs
    int display_w, display_h;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwGetFramebufferSize((GLFWwindow *)window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EndFrame(double deltaTime)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    MainMenu(1.0f / deltaTime);
    MainDockSpace();

    auto [ux, uy] = Lotus::Renderer::GetViewportUV();
    Viewport(colorBuffer, ux, uy);

    SceneTree();
    Properties();
    Control();
    //        Log();

    // Rendering
    ImGui::Render();
    renderWidgets();
}

void Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

/** Extra widgets that don't really belong in their own files */

void MainDockSpace()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}
} // namespace Editor::Widgets
