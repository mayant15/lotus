#pragma once

#include "Material.h"
#include <lotus/rendering/common.h>

namespace Lotus
{
    struct Model
    {
        std::vector<Geometry> Meshes;

        struct {
            std::string path;
        } detail;
    };

    LOADER(Model)
    {
        // TODO: Save the model as a lotus json too. We should not have to rely on Assimp for material data other than the first import.
        //  there is a need to separate "import" and "load".
        [[nodiscard]] SRef<Model> Load(const std::string& relpath) const;
    };
}
