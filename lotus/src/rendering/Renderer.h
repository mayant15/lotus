#pragma once

#include <lotus/ILifecycle.h>
#include <lotus/resources/Shader.h>
#include <lotus/ecs/components/CTransform.h>
#include <lotus/CMeshRenderer.h>

namespace Lotus::Renderer
{
    /** @brief Structure that backs the render pipeline */
    struct State
    {
        unsigned int ViewportWidth;
        unsigned int ViewportHeight;
        double AspectRatio;

        Vector3f CameraPos;
        Matrix4f View;
        Matrix4f Projection;
    };

    /**
     * @brief Setup state and config options
     * @param event
     */
    void OnInit(const InitEvent& event);

    /**
     * @brief Register render passes before the main loop starts
     *
     * This is done in OnBegin and not in OnInit because render passes need the scene to be populated in
     * their constructors, and they then store a few references
     *
     * @param event
     */
    void OnBegin(const BeginEvent& event);

    /**
     * @brief Find the camera and setup some options that all render passes will need
     * @param event
     */
    void OnPreUpdate(const PreUpdateEvent& event);

    /**
     * @brief Execute all render passes in order
     * @param event
     */
    void OnUpdate(const UpdateEvent& event);

    /**
     * @brief Delete the render pipeline
     * @param event
     */
    void OnDestroy(const DestroyEvent& event);
}
