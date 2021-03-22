#include <lotus/lotus.h>
#include <lotus/debug.h>
#include <quickstart_export.h>

#define BALL_TAG "ball"

struct CTag
{
    std::string Tag;
    GENERATED_BODY(CTag, Tag);
};

namespace DestroyBall {
    using namespace Lotus;

    static EntityID ballID;
    static bool destroyed = false;

    void OnBegin(const BeginEvent& event)
    {
        auto* reg = GetRegistry();
        auto view = reg->view<CTag>();
        for (auto id : view)
        {
            auto& tag = view.get(id);
            if (tag.Tag == BALL_TAG)
            {
                ballID = id;
                break;
            }
        }
    }

    void OnUpdate(const UpdateEvent& event)
    {
        if (!destroyed)
        {
            auto* reg = GetRegistry();
            auto&& [tag, transform] = reg->get<CTag, CTransform>(ballID);
            if (tag.Tag == BALL_TAG && transform.Position.y <= 4)
            {
                reg->destroy(ballID, reg->current(ballID));
                destroyed = true;
                LOG_INFO("Destroyed the ball");
            }
        }
    }
}

//{
//"CTransform": {
//"Position": [0.0, 0.0, 0.0],
//"Rotation": [0.0, -90.0, 0.0],
//"Scale": [15.0, 0.3, 15.0]
//},
//"CMeshRenderer": {
//"Material": "res://materials/default.json",
//"Model": "res://mesh/cube.fbx"
//},
//"CBoxCollider": {
//"Position": [0.0, 0.0, 0.0],
//"Dimensions": [15.0, 0.3, 15.0]
//},
//"CRigidBody": {
//"Gravity": 1.0,
//"IsKinematic": true
//}
//},

void OnContact(const Lotus::CollisionEvent& event)
{
    LOG_INFO("Collision!");
}

//=============================================================================
// REGISTRATION FUNCTIONS
//=============================================================================

QUICKSTART_API void ModuleTestFunction()
{
    LOG_INFO("Loading [module:quickstart]");
}

QUICKSTART_API void RegisterEvents()
{
    using namespace Lotus;

    LOG_INFO("Registering [module:quickstart]");

    EventManager& em = GET(EventManager);
    em.Bind<UpdateEvent, DestroyBall::OnUpdate>();
    em.Bind<BeginEvent, DestroyBall::OnBegin>();
    em.Bind<CollisionEvent, OnContact>();

    RegisterComponent<CTag>();
}
