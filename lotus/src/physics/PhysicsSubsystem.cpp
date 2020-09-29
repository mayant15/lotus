#include "PhysicsSubsystem.h"

#include "lotus/debug.h"

#define PX_RELEASE(x) if(x) { x->release(); x = nullptr; }
#define PVD_HOST "127.0.0.1"

namespace Lotus::Physics
{
    PhysicsSubsystem::PhysicsSubsystem(bool usePVD)
    {
        LOG_INFO("Creating physics world");
        _pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

        if (usePVD)
        {
            _pPVD = PxCreatePvd(*_pFoundation);
            PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
            _pPVD->connect(*transport, PxPvdInstrumentationFlag::eALL);
        }

        _pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *_pFoundation, PxTolerancesScale(), true, _pPVD);
    }

    void PhysicsSubsystem::CreateScene(const PhysicsSceneInfo& info)
    {
        LOG_INFO("Creating physics scene");


        PxSceneDesc sceneDesc(_pPhysics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(info.Gravity.x, info.Gravity.y, info.Gravity.z);

        _pDispatcher = PxDefaultCpuDispatcherCreate(2);
        sceneDesc.cpuDispatcher = _pDispatcher;
        sceneDesc.filterShader = PxDefaultSimulationFilterShader;

        _pActiveScene = _pPhysics->createScene(sceneDesc);

        PxPvdSceneClient* pvdClient = _pActiveScene->getScenePvdClient();
        if (_pPVD && pvdClient)
        {
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }

        // TODO: Add geometry to scene
        const auto defMat = _pPhysics->createMaterial(0.5f, 0.5f, 0.6f);
        const PxSphereGeometry geom(1.0f);
        const PxTransform transform(PxVec3(1.0f, 1.0f, 1.0f));

        PxShape* shape = _pPhysics->createShape(geom, *defMat, true);
        PxRigidDynamic* actor = PxCreateDynamic(*_pPhysics, transform, *shape, 1.0f);
        _pActiveScene->addActor(*actor);
        
        // PxRigidStatic* groundPlane = PxCreatePlane(*_pPhysics, PxPlane(0, 1, 0, 0), *defMat);
        // _pActiveScene->addActor(*groundPlane);
    }

    void PhysicsSubsystem::Update(float delta) const
    {
        _pActiveScene->simulate(delta);
        _pActiveScene->fetchResults(true);
    }

    void PhysicsSubsystem::Shutdown()
    {
        LOG_INFO("Destroying Physics...");
        PX_RELEASE(_pActiveScene)
            PX_RELEASE(_pDispatcher)
            PX_RELEASE(_pPhysics)
            if (_pPVD)
            {
                PxPvdTransport* transport = _pPVD->getTransport();
                _pPVD->release();
                _pPVD = nullptr;
                PX_RELEASE(transport)
            }
        PX_RELEASE(_pFoundation)
    }
}
