#include <lotus/lotus.h>
#include <lotus/debug.h>
#include <pong_export.h>

// TODO: Attaching scripts to entities
//   call events as void OnSceneLoad(Entity entity, const SceneLoadEvent& event)

void MoveBallOnStart(const Lotus::SimulationBeginEvent& e)
{
    static constexpr float BALL_SPEED = 0.01;

    using namespace Lotus;
    auto scene = SceneManager::GetCurrentScene();
    auto ball = scene->GetEntity("Ball");
    auto& rb = ball.GetComponent<CRigidBody>();
    Physics::ApplyForce(rb, Vector3f {1.0, 0.0, -0.2} * BALL_SPEED, Physics::EForceType::IMPULSE);
}

void MovePaddleOnUpdate(const Lotus::UpdateEvent& e)
{
    static constexpr float PADDLE_SPEED = 5.0;

    using namespace Lotus;
    if (Input::GetKeyPressed(L_KEY_A) || Input::GetKeyPressed(L_KEY_D))
    {
        bool is_right = Input::GetKeyPressed(L_KEY_D);
        float value = (is_right ? 1.0 : -1.0) * e.DeltaTime * PADDLE_SPEED;

        auto scene = SceneManager::GetCurrentScene();
        auto paddle = scene->GetEntity("Player");

        paddle.PatchComponent<CTransform>([&](auto& tf) {
            tf.Position += GetRightVector(tf) * value;
        });
    }
}

//=============================================================================
// REGISTRATION FUNCTIONS
//=============================================================================

PONG_API void ModuleTestFunction()
{
    LOG_INFO("Loading [module:pong]");
}

PONG_API void RegisterEvents()
{
    using namespace Lotus;
    LOG_INFO("Registering [module:pong]");

    auto& em = EventManager::Get();
    em.Bind<SimulationBeginEvent, MoveBallOnStart>();
    em.Bind<UpdateEvent, MovePaddleOnUpdate>();
}
