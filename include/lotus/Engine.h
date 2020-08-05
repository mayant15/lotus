#pragma once

#include "lcommon.h"
#include "rendering/Renderer.h"

namespace Lotus
{
    struct LotusOp
    {
        ERenderAPI RenderAPI = ERenderAPI::OPEN_GL;
        bool IsDebug = true;
        unsigned int Width = 800;
        unsigned int Height = 600;
    };

    class Engine : public Singleton<Engine>
    {
        // TODO: Wrap the raw pointer, like in Scene
        Renderer* _renderer;
    private:
        friend Singleton<Engine>;

        Engine() = default;

        void tick();

    public:
        void Initialize(const LotusOp& options);

        void Run();

        void Shutdown();
    };
}
