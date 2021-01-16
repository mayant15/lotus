#include <widgets.h>

namespace Editor::Widgets
{
    ImVec2 viewportDims {1280.0f, 720.0f};
    static bool show = true;

    void Viewport(unsigned int texture, float ux, float uy)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});

        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoScrollbar;
        ImGui::Begin("Viewport", &show, flags);

        // NOTE: The viewport setup is kind of convoluted. The color buffer size always stays constant.
        // When the viewport size changes, the portion of the color buffer that is rendered to changes, while maintaining
        // the aspect ratio. This portion is controlled through glViewport, and the logic is in the renderer. See
        // Lotus::Renderer::SetViewport and Lotus::Renderer::OnUpdate
        //
        // Then, Lotus::Renderer::GetViewportUV returns the portion of the color buffer that is rendered to, normalized
        // to be a fraction, i.e, if [0, 1]x[0, 1] is the color buffer, then [0, ux]x[0, uy] is the rendered region.
        // This is then passed to imgui as the UVs to map to the window, essentially "cropping" the texture.
        // TODO: Is this faster than blitting to another texture? Idk, I'll probably have to create a new texture to blit
        //   to every frame in order to account for the changing viewport size? That's why I implemented it this way
        ImGui::GetWindowDrawList()->AddImage(
                (void*) texture,
                ImVec2(ImGui::GetCursorScreenPos()),
                ImVec2(ImGui::GetCursorScreenPos().x + viewportDims.x,
                       ImGui::GetCursorScreenPos().y + viewportDims.y), ImVec2(0, uy), ImVec2(ux, 0));

        // Update viewport dims to render the next frame
        viewportDims = ImGui::GetContentRegionAvail();

        ImGui::End();
        ImGui::PopStyleVar();
    }

    ImVec2 GetViewportDims()
    {
        return viewportDims;
    }
}