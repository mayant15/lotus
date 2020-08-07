#pragma once

#include "lotus/events/Window.h"
#include "lotus/components/rendering.h"
#include "lotus/components/scene.h"

namespace Lotus
{
    struct RendererOp
    {
        bool IsDebug;
        ERenderAPI RenderAPI;
        uint32_t ViewportWidth;
        uint32_t ViewportHeight;
    };

    class Renderer : public ILifecycle
    {
    protected:
        RendererOp _options{};

    public:
        virtual void Initialize(const RendererOp& options)
        {}

        virtual void SetViewport(int x, int y, int width, int height)
        {}

        virtual void DrawMesh(const CMeshRenderer& data, const CTransform& transform)
        {}

        void OnPreUpdate() override = 0;

        void OnUpdate(float delta) override = 0;

        void OnPostUpdate() override = 0;

        void OnPreDestroy() override = 0;

        void OnDestroy() override = 0;
    };
}
