#pragma once

#include "lotus/lcommon.h"

namespace Lotus
{
    struct LOTUS_API Cubemap
    {
        unsigned int ID = 0;
        unsigned int VAO = 0;
        unsigned int VBO = 0;
    };

    LOADER(CubemapLoader, Cubemap)
    {
        /**
         * Load a cubemap with six images that are to be used as the cube's faces.
         */
        SRef<Cubemap> Load(const std::vector<std::string>& faces) const;

        /**
         * Load an HDRI that is first loaded as a regular texture and then converted to a cubemap.
         */
        SRef<Cubemap> Load(const std::string& hdri) const;
    };
}
