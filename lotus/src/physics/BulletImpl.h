#pragma once

#define BT_USE_DOUBLE_PRECISION

#include <LinearMath/btQuaternion.h>
#include <LinearMath/btScalar.h>
#include <LinearMath/btVector3.h>

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <lotus/physics/Vector.h>

///////////////////////////////////////////////////////////////////////////////

namespace Lotus::Physics {

class World
{
  public:
    World();
    ~World();

    void step(double deltaTime);
    void setGravity(const Vector3f &vec);

  private:
    btDiscreteDynamicsWorld *pWorldImpl_ = nullptr;
    btDefaultCollisionConfiguration *pConfig_ = nullptr;
    btCollisionDispatcher *pDispatcher_ = nullptr;
    btBroadphaseInterface *pBroadphase_ = nullptr;
    btSequentialImpulseConstraintSolver *pSolver_ = nullptr;
};

} // namespace Lotus::Physics
