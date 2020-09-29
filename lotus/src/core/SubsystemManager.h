#pragma once

#include "lotus/lcommon.h"
#include "physics/PhysicsSubsystem.h"

namespace Lotus
{
    /**
     * @brief Helper class for the engine to manage independent subsystems.
    */
    class SubsystemManager : public Singleton<SubsystemManager>
    {
        URef<Physics::PhysicsSubsystem> _pPhysics;
        // RenderSubsystem* _pRenderer;
        // EntitySubsystem* _pEntity;

        SubsystemManager();
        friend Singleton<SubsystemManager>;

    public:
        void Start() const;
        void Update(float delta) const;
        void Shutdown() const;

        // TODO: Return reference vs return pointer
        [[nodiscard]] Physics::PhysicsSubsystem& GetPhysics() const;
    };
}
