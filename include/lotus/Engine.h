#pragma once

#include "lcommon.h"
#include "rendering/Renderer.h"
#include "events/EventDispatcher.h"

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
        URef<Window> _window;
        // TODO: Wrap the raw pointer, like in Scene
        Renderer* _renderer {};
        bool _isRunning = true;

    private:
        friend Singleton<Engine>;

        Engine() = default;

        void tick();

    public:
        void Initialize(const LotusOp& options);

        void Run();

        void Stop();

        void Shutdown();
    };
}
