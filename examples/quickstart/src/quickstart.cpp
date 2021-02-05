#include <quickstart.h>

#define BALL_TAG "ball"

struct CTag
{
    std::string name;
};

namespace DestroyBall {
    using namespace Lotus;

    static EntityID ballID;

    void OnBegin(const BeginEvent& event)
    {
        auto* reg = GetRegistry();
        auto view = reg->view<CTag>();
        for (auto id : view)
        {
            auto& tag = view.get(id);
            if (tag.name == BALL_TAG)
            {
                ballID = id;
                break;
            }
        }
    }

    void OnUpdate(const UpdateEvent& event)
    {
        auto* reg = GetRegistry();
        auto&& [tag, transform] = reg->get<CTag, CTransform>(ballID);
        if (tag.name == BALL_TAG && transform.Position.y <= 0)
        {
            reg->destroy(ballID, reg->current(ballID));
        }
    }
}

void OnContact(const Lotus::CollisionEvent& event)
{
    LOG_INFO("Collision!");
}

void ModuleTestFunction()
{
    LOG_INFO("Loaded quickstart.dll");
}

void RegisterEvents()
{
    using namespace Lotus;

    LOG_INFO("Registering events!");

    EventManager& em = GET(EventManager);
//    em.Bind<UpdateEvent, DestroyBall::OnUpdate>();
//    em.Bind<BeginEvent, DestroyBall::OnBegin>();
    em.Bind<CollisionEvent, OnContact>();
}
