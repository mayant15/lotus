#include "SubsystemManager.h"

namespace Lotus
{
    SubsystemManager::SubsystemManager()
    {
        _pPhysics = std::make_unique<Physics::PhysicsSubsystem>();
    }

    void SubsystemManager::Start() const
    {
        // TODO: Create scenes through the scene manager
        _pPhysics->CreateScene(Physics::PhysicsSceneInfo()); // default scene info
    }

    void SubsystemManager::Update(float delta) const
    {
        _pPhysics->Update(delta);
    }

    void SubsystemManager::Shutdown() const
    {
        _pPhysics->Shutdown();
    }

    Physics::PhysicsSubsystem& SubsystemManager::GetPhysics() const
    {
        return *_pPhysics;
    }
}
