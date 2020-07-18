#pragma once
#include <memory>
#include <vector>
#include "lotus/rendering/Camera.h"
#include "lotus/scene/objects.h"

class Scene {
    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<Actor>> actors;
    std::vector<std::shared_ptr<ALight>> lights;

public:
    void addCamera(glm::vec3 posVec, glm::vec3 worldUpVec = glm::vec3(0.0f, 1.0f, 0.0f));
    void addActor(glm::vec3 position_, const CMesh& mesh);
    void addLight(glm::vec3 position, const CMesh& meshComponent);

    [[nodiscard]] std::shared_ptr<Camera> getCamera() const;
    [[nodiscard]] std::vector<std::shared_ptr<Actor>> getActors() const;
    [[nodiscard]] std::vector<std::shared_ptr<ALight>> getLights() const;
};
