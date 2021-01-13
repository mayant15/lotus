#pragma once

#include <lotus/lcommon.h>

namespace Lotus::Renderer
{
    /**
     * @brief A single step executed by the rendering pipeline
     *
     * A RenderPass is a way to organize tasks in the render pipeline. This provides a base class that implementations
     * will derive from to define rendering tasks. The rendering pipeline simply executes these passes in the order
     * that they are registered.
     */
    class LOTUS_API RenderPass
    {
    public:
        /** @brief Called on PreUpdateEvent */
        virtual void SetupFrame() {};

        /** @brief Called on UpdateEvent */
        virtual void RenderFrame(double deltaTime) {};

        virtual ~RenderPass() = default;
    };

    LOTUS_API void AddRenderPass(RenderPass* pass);
}
