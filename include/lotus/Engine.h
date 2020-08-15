#pragma once

#include "lcommon.h"
#include "rendering/Renderer.h"
#include "events/EventManager.h"

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
        bool _isRunning = true;
        URef<Window> _window;
        EventManager* _eventManager{};
        Renderer* _renderer{};
    private:
        friend Singleton<Engine>;

        Engine() = default;

        void tick(float delta);

        // This guy needs to be a reference so that a single callback can be used for all window events
        // This function should then typecast into an appropriate type before ending the thing to the Event Manager.
        static void OnEvent(Event& event);

    public:

        void Initialize(const LotusOp& options);

        void Run();

        void Shutdown();
    };
}
