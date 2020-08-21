#pragma once

#include "lcommon.h"
#include "Renderer.h"
#include "events/EventManager.h"
#include "events/Input.h"

namespace Lotus
{
    /**
     * Options to initialize the engine with.
     */
    struct LOTUS_API LotusOp
    {
        ERenderAPI RenderAPI = ERenderAPI::OPEN_GL;
        bool IsDebug = true;
        unsigned int Width = 800;
        unsigned int Height = 600;
    };

    /**
     * Core Lotus engine class that handles all subsystems and the main gameplay loop.
     */
    class LOTUS_API Engine : public Singleton<Engine>
    {
        bool _isRunning = true;

        // NOTE: While this is a unique pointer owned by the engine, the underlying object can be modified
        // by the lifecycle events called on it.
        URef<IWindow> _window;

        EventManager* _eventManager{};
        Input* _inputManager{};
        Renderer* _renderer{};

    public:
        /**
         * Initialize the engine and setup core subsystems.
         * @param options Options to configure the engine with
         */
        void Initialize(const LotusOp& options);

        /**
         * Start the main gameplay loop.
         */
        void Run();

        /**
         * Shutdown the engine.
         */
        void Shutdown();

    private:
        friend Singleton<Engine>;

        Engine() = default;

        void tick(float delta);

        // This guy needs to be a reference so that a single callback can be used for all window events
        // This function should then typecast into an appropriate type before ending the thing to the Event Manager.
        static void OnEvent(Event& event);
    };
}
