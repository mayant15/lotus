#pragma once

#include "window/Window.h"
#include <lotus/rendering/RenderPass.h>

namespace Editor::Gizmos
{
    class GizmoPass final : public Lotus::Renderer::RenderPass
    {
    public:
        GizmoPass() = default;

        void SetupFrame() override;
        void RenderFrame(double deltaTime) override;
    };

    void Initialize(Window* window);
    void Shutdown();
}
