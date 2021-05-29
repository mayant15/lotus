#include <lotus/lotus.h>
#include <lotus/debug.h>
#include <pong_export.h>

// TODO: Attaching scripts to entities
//   call events as void OnSceneLoad(Entity entity, const SceneLoadEvent& event)

static constexpr double PADDLE_SPEED = 5.0;

void MovePaddleOnUpdate(const Lotus::UpdateEvent& e)
{
    using namespace Lotus;
    if (Input::GetKeyPressed(L_KEY_A) || Input::GetKeyPressed(L_KEY_D))
    {
        bool is_right = Input::GetKeyPressed(L_KEY_D);
        float value = (is_right ? 1.0 : -1.0) * e.DeltaTime * PADDLE_SPEED;

        auto scene = SceneManager::GetCurrentScene();
        auto paddle = scene->GetEntity("Player");
        auto& tf = paddle.GetComponent<CTransform>();

        tf.Position += GetRightVector(tf) * value;
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
    em.Bind<UpdateEvent, MovePaddleOnUpdate>();
}
