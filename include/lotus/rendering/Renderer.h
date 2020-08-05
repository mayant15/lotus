#pragma once

#include "lotus/components.h"
#include "lotus/resources.h"
#include "Window.h"
#include "LShader.h"

namespace Lotus
{
    struct RendererOp
    {
        bool IsDebug;
        ERenderAPI RenderAPI;
        unsigned int Width;
        unsigned int Height;
    };

    struct CMeshRenderer
    {
        // Render data
        SRef<LShader> Shader;
        SRef<LModel> Model;
    };

    // TODO: Find a better way to keep this class abstract and enforce override restrictions at the same time
    class Renderer : public ILifecycle
    {
    public:
        virtual URef<Window>& GetActiveWindow() = 0;

        virtual void Initialize(const RendererOp& options)
        {}

        virtual void SetViewport(int x, int y, int width, int height)
        {}

        virtual void DrawMesh(const CMeshRenderer& data, const CTransform& transform)
        {}

        void OnPreUpdate() override
        {}

        void OnUpdate(float delta) override = 0;

        void OnPostUpdate() override
        {}

        void OnPreDestroy() override
        {}

        void OnDestroy() override
        {}
    };
}
