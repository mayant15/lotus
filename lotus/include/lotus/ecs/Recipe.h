#pragma once

#include "lotus/lcommon.h"
#include "lotus/ecs/components/IComponentInfo.h"

namespace Lotus
{
    /**
     * @brief Description of how to build an entity
     *
     * Equivalent to a prefab
     */
    class LOTUS_API Recipe
    {
        std::vector<IComponentInfo> _components;

    public:
        Recipe(const std::string& path);

        [[nodiscard]] auto begin() const noexcept
        {
            return _components.begin();
        }

        [[nodiscard]] auto end() const noexcept
        {
            return _components.end();
        }
    };
}