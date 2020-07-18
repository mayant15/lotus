#pragma once
#include <vector>
#include "lotus/scene/components.h"

class Object
{
    std::vector<Component*> components;

    template<typename T>
    bool getComponentType(Component* pComponent);

public:
    bool isActive = true;

    template<typename T>
    Component getComponent();

    virtual void start() = 0;

    virtual void update() = 0;
};

class Actor : public Object
{
protected:
    CTransform transform;
    CMesh mesh;

public:
    Actor(glm::vec3 position_, const CMesh& meshComponent_);

    const CMesh& getMesh() const;

    void update() override;

    void start() override;
};

class ALight : public Actor {
public:
    CPointLight light{};
    ALight(glm::vec3 position, const CMesh& meshComponent);
};