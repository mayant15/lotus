#pragma once

#include <lotus/lotus_export.h>
#include <string>

namespace Lotus
{
    /**
     * @brief Resolve a given relative path
     *
     * This function can be used to resolve paths relative to the configured project
     * or engine resource roots. Use @c res:// to refer to project resources and
     * @c int:// to refer to engine resources.
     *
     * @param path Relative path
     * @return string Absolute counterpart to the given path
     */
    LOTUS_API std::string ExpandPath(const std::string& path);
}
