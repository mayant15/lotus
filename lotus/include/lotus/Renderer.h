#pragma once

#include <lotus/ILifecycle.h>
#include <lotus/resources/Model.h>
#include <lotus/resources/Shader.h>
#include <lotus/ecs/components/CTransform.h>
#include <lotus/filesystem.h>
#include <lotus/Config.h>

namespace Lotus
{
    struct CMeshRenderer
    {
        Handle<Material> MeshMaterial;
        Handle<Model> MeshModel;
        // NOTE: These cannot be just "Material" or "Model" because GCC complains about the name
        // MSVC and clang will work fine though

        REGISTER_DECL();
    };

    inline void to_json(nlohmann::json& data, const CMeshRenderer& mr)
    {
        // TODO: Handle to path
    }

    inline void from_json(const nlohmann::json& data, CMeshRenderer& mr)
    {
        mr.MeshModel = LoadAsset<Model, ModelLoader>(ExpandPath(data.at("Model").get<std::string>()));
        mr.MeshMaterial = LoadAsset<Material, MaterialLoader>(ExpandPath(data.at("Material").get<std::string>()));
    }

    REGISTER_BODY(CMeshRenderer);


    /**
     * Renderer class used by the end user. Abstracts away platform specific implementation details.
     * All implementations must subclass this.
     */
    class Renderer : public ILifecycle
    {
    protected:
        RenderConfig _options {};

    public:
        /**
         * Initialize the renderer.
         * @param options Options to configure the renderer
         */
        virtual void Initialize(const RenderConfig& options)
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
