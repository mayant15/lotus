#include "lotus/ecs/Recipe.h"
#include "lotus/debug.h"

namespace Lotus
{
    using namespace nlohmann;
    Recipe::Recipe(const std::string &path)
    {
        LOG_INFO("Loading recipe at {}", path);
        std::ifstream file (path);
        json data;
        file >> data;

        if (!data.is_array())
        {
            throw std::invalid_argument("Given path is not a valid recipe" + path);
        }

        for (auto component : data)
        {
            IComponentInfo info;
            info.Name = component['name'];
            _components.push_back(info);
        }
    }
}
