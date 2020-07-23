#pragma once

namespace Lotus
{
    void init();

    void run();

    void shutdown();

    struct IComponent
    {
    };

    class LObject
    {
        // TODO: Component lists?

    public:
        bool isActive = true;

        // TODO: Implement
        virtual void start() = 0;

        virtual void update() = 0;
    };

}
