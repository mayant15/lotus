#pragma once

#include "lotus/lcommon.h"
#include "lotus/ecs/EventManager.h"
#include "lotus/ecs/Event.h"

#include "core/IWindow.h"
#include "ecs/SystemRegistry.h"

namespace Lotus
{
    /**
     * Core Lotus engine class that handles all subsystems and the main gameplay loop.
     */
    class Engine : public Singleton<Engine>
    {
        bool _isRunning = true;

        // NOTE: While this is a unique pointer owned by the engine, the underlying object can be modified
        // by the lifecycle events called on it.
        URef<IWindow> _window;
        URef<SystemRegistry> _systemRegistry;

    public:
        /**
         * Initialize the engine and setup core subsystems.
         * @param options Options to configure the engine with
         */
        void Initialize();

        /**
         * Start the main gameplay loop.
         */
        void Run();

        /**
         * Shutdown the engine.
         */
        void Shutdown();

        void OnWindowClose(const WindowCloseEvent& event);

    private:
        friend Singleton<Engine>;

        Engine() = default;

        void tick(float delta);
    };
}
