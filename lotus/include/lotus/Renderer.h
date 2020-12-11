#pragma once

#include "lotus/ILifecycle.h"
#include "lotus/resources/Model.h"
#include "lotus/resources/Shader.h"
#include "lotus/ecs/components/CTransform.h"

namespace Lotus
{
    /**
     * Options to initialize the renderer with.
     */
    struct RendererOp
    {
        /**
         * Should create a debug context
         */
        bool IsDebug;

        /**
         * Render API that is to be used
         */
        ERenderAPI RenderAPI;

        /**
         * Width of the created viewport
         */
        uint32_t ViewportWidth;

        /**
         * Height of the created viewport
         */
        uint32_t ViewportHeight;
    };

    struct CMeshRenderer
    {
        // Render data
        Handle<Shader> Shader;
        Handle<Model> Model;

        REGISTER_DECL();
    };

    inline void to_json(nlohmann::json& data, const CMeshRenderer& mr)
    {
        // TODO: Handle to path
    }

    inline void from_json(const nlohmann::json& data, CMeshRenderer& mr)
    {
        mr.Model = LoadAsset<Model, ModelLoader>(data.at("Model").get<std::string>());
        mr.Shader = LoadAsset<Shader, ShaderLoader>(
                data.at("Shader").at("Vertex").get<std::string>(),
                data.at("Shader").at("Fragment").get<std::string>()
        );
    }

    REGISTER_BODY(CMeshRenderer);


    /**
     * Renderer class used by the end user. Abstracts away platform specific implementation details.
     * All implementations must subclass this.
     */
    class Renderer : public ILifecycle
    {
    protected:
        RendererOp _options{};

    public:
        /**
         * Initialize the renderer.
         * @param options Options to configure the renderer
         */
        virtual void Initialize(const RendererOp& options)
        {}

        /**
         * Set viewport dimensions
         * @param x Origin x-coordinate
         * @param y Origin y-coordinate
         * @param width Desired viewport width
         * @param height Desired viewport height
         */
        virtual void SetViewport(int x, int y, int width, int height)
        {}

        /**
         * Draw a mesh
         * @param data CMeshRenderer object reference holding data for the mesh to be drawn
         * @param transform CTransform reference for the mesh to be drawn
         */
        virtual void DrawMesh(const CMeshRenderer& data, const CTransform& transform)
        {}

        void OnPreUpdate(const PreUpdateEvent& event) override = 0;
        void OnUpdate(const UpdateEvent& event) override = 0;
    };
}
