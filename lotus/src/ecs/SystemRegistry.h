#pragma once

#include "lotus/lcommon.h"
#include "lotus/Config.h"
#include "lotus/Renderer.h"
#include "lotus/Input.h"

#include "physics/PhysicsSubsystem.h"

namespace Lotus
{
    /**
     * @brief Helper class for the engine to manage independent subsystems.
     *
     * Subsystems in lotus are intended to run separately, like physics and rendering.
     * These subsystems should know nothing about each other and have independent ticks.
     * This class coordinates between these subsystems and passes relevant information.
     * This class also interfaces with the main Engine class to provide a top-level handle
     * over all subsystems.
    */
    class SystemRegistry
    {
        Renderer* _renderer;
        URef<Physics::PhysicsSubsystem> _physics;

    public:
        /**
         * @brief Starts all subsystems
        */
        SystemRegistry();

        /**
         * @brief Shutdown all subsystems
         * TODO: This only handles ECS for now. Move that to an event-based thing too
        */
        void Shutdown() const;
    };
}
