#include "BulletImpl.h"

///////////////////////////////////////////////////////////////////////////////

namespace Lotus::Physics {

static inline Vector3f toVector3f(const btVector3 &vec)
{
    return {vec.x(), vec.y(), vec.z()};
}

static inline btVector3 toBtVector3(const Vector3f &vec)
{
    return {vec.x, vec.y, vec.z};
}

World::World()
{
    pConfig_ = new btDefaultCollisionConfiguration();
    pDispatcher_ = new btCollisionDispatcher(pConfig_);
    pBroadphase_ = new btDbvtBroadphase();
    pSolver_ = new btSequentialImpulseConstraintSolver();
    pWorldImpl_ = new btDiscreteDynamicsWorld(pDispatcher_, pBroadphase_, pSolver_, pConfig_);
}

void World::step(double deltaTime)
{
    pWorldImpl_->stepSimulation(deltaTime);
}

void World::setGravity(const Vector3f &vec)
{
    pWorldImpl_->setGravity(toBtVector3(vec));
}

World::~World()
{
    delete pWorldImpl_;
    delete pSolver_;
    delete pBroadphase_;
    delete pDispatcher_;
    delete pConfig_;
}

} // namespace Lotus::Physics
