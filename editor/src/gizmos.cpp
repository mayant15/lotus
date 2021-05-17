#include "gizmos.h"
#include <lotus/rendering/common.h>

namespace Editor::Gizmos
{
    void Initialize(Window* window)
    {
        // Register the gizmo render pass
        Lotus::Renderer::AddRenderPass(new GizmoPass);
    }

    void Shutdown()
    {
        // Cleanup?
    }

    void GizmoPass::SetupFrame()
    {
        //
    }

    void GizmoPass::RenderFrame(double deltaTime)
    {
        //
    }
}
