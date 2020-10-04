#pragma once

#include "lotus/lcommon.h"
#include "lotus/Config.h"
#include "lotus/Renderer.h"
#include "lotus/Input.h"

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
        Input* _input;
//        std::unordered_map<std::string, ISystem> _userSystems;

    public:
        /**
         * @brief Starts all subsystems
        */
        void Initialize(const Config& config);

        /**
         * @brief Run the tick for each subsystem
         * @param delta timestep
         * TODO: Ticks should run independently
        */
        void Update(float delta) const;

        /**
         * @brief Shutdown all subsystems
        */
        void Shutdown() const;
    };
}
