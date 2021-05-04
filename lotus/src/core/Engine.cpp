#include <lotus/Engine.h>
#include <physics/Physics.h>
#include <rendering/Renderer.h>
#include <lotus/ecs/EventManager.h>
#include <lotus/scene/SceneManager.h>
#include <lotus/debug.h>

namespace Lotus::Engine
{
    /** @brief Register components provided by the engine */
    void setupComponents()
    {
        auto& em = GET(EventManager);
        RegisterEngineComponents();
        em.Bind<SimulationBeginEvent, SceneManager::OnSimulationBegin>();
        em.Bind<SimulationEndEvent, SceneManager::OnSimulationEnd>();
    }

    /** @brief Setup the physics subsystem and register events */
    void setupPhysics()
    {
        auto& eventManager = GET(EventManager);
        eventManager.Bind<InitEvent, Physics::OnInit>();
        eventManager.Bind<SceneLoadEvent, Physics::OnSceneLoad>();
        eventManager.Bind<SimulationBeginEvent, Physics::OnSimulationBegin>();
        eventManager.Bind<SimulationPauseEvent, Physics::OnSimulationPause>();
        eventManager.Bind<SimulationEndEvent, Physics::OnSimulationEnd>();
        eventManager.Bind<PreUpdateEvent, Physics::OnPreUpdate>();
        eventManager.Bind<UpdateEvent, Physics::OnUpdate>();
        eventManager.Bind<PostUpdateEvent, Physics::OnPostUpdate>();
        eventManager.Bind<DestroyEvent, Physics::OnDestroy>();
        eventManager.Bind<ComponentCreateEvent<CRigidBody>, Physics::OnRigidBodyCreate>();
        eventManager.Bind<ComponentDestroyEvent<CRigidBody>, Physics::OnRigidBodyDestroy>();
    }

    /** @brief Setup the platform-specific renderer and register events */
    void setupRenderer()
    {
        auto& eventManager = GET(EventManager);
        eventManager.Bind<InitEvent, Renderer::OnInit>();
        eventManager.Bind<SceneLoadEvent, Renderer::OnSceneLoad>();
        eventManager.Bind<PreUpdateEvent, Renderer::OnPreUpdate>();
        eventManager.Bind<UpdateEvent, Renderer::OnUpdate>();
        eventManager.Bind<DestroyEvent, Renderer::OnDestroy>();
        eventManager.Bind<ComponentCreateEvent<CMeshRenderer>, Renderer::OnMeshRendererCreate>();
    }

    void Initialize()
    {
        setupComponents();
        setupPhysics();
        setupRenderer();

        auto& eventManager = GET(EventManager);
        eventManager.Dispatch(InitEvent {});
    }

    /** @brief Perform an engine tick and dispatch update events. */
    void Tick(double deltaTime)
    {
        auto& eventManager = GET(EventManager);

        eventManager.Dispatch(PreUpdateEvent {});

        UpdateEvent ue;
        ue.DeltaTime = deltaTime;
        eventManager.Dispatch(ue);

        eventManager.Dispatch(PostUpdateEvent {});

        // Dispatch remaining queued events
        eventManager.DispatchAll();
    }

    void Shutdown()
    {
        LOG_INFO("Shutting down engine...");
        auto& eventManager = GET(EventManager);
        eventManager.Dispatch(PreDestroyEvent {});
        eventManager.Dispatch(DestroyEvent {});
        eventManager.Dispatch(ShutdownEvent {});
    }
}
