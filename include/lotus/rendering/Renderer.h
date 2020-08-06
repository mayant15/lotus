#pragma once

#include "Window.h"
#include "lotus/components/rendering.h"
#include "lotus/components/scene.h"

namespace Lotus
{
    struct RendererOp
    {
        bool IsDebug;
        ERenderAPI RenderAPI;
        WindowOp WindowOptions;
    };

    // TODO: Find a better way to keep this class abstract and enforce override restrictions at the same time
    class Renderer : public ILifecycle
    {
    protected:
        URef<Window> _window;
        RendererOp _options{};

    public:
        URef<Window>& GetActiveWindow()
        {
            return _window;
        }

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
