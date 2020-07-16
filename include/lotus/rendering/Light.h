#include <glm/vec3.hpp>

struct Light {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight : Light {
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight : Light {
    glm::vec3 direction;
};

struct Spotlight : Light {
    glm::vec3 position;
    glm::vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float innerCutOff;
    float outerCutOff;
};
