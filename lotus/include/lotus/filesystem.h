#pragma once

#include <lotus/lotus_export.h>
#include <string>

namespace Lotus
{
    LOTUS_API std::string GetProjectResourceRoot();
    LOTUS_API std::string GetEngineResourceRoot();
    LOTUS_API void SetProjectResourceRoot(const std::string& path);
    LOTUS_API std::string ExpandPath(const std::string& path);
}
