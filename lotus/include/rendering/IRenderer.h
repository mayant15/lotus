#pragma once

// TODO: Move scene to private dependencies
#include "lotus/scene.h"
#include "rendering/Window.h"

class IRenderer
{
protected:
    URefWindow window;

public:
    virtual URefWindow& getActiveWindow() = 0;

    virtual void init(bool isDebug) = 0;

    virtual void renderScene(const Lotus::Scene& scene) = 0;

    virtual void update() = 0;

    virtual void shutdown() = 0;
};
