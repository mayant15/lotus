#include <filesystem>

#include <lotus/filesystem.h>
#include <lotus/debug.h>

namespace Lotus
{
    static std::string projectResourceRoot;
    static const std::string engineResourceRoot = ENGINE_RESOURCE_ROOT;

    void SetProjectResourceRoot(const std::string& path)
    {
        projectResourceRoot = path;
    }

    std::string ExpandPath(const std::string& path)
    {
        std::filesystem::path arg = path;
        if (arg.is_absolute())
        {
            return path;
        }

        // If path starts with res://, append user resource root
        if (path.substr(0, 6) == "res://")
        {
            return projectResourceRoot + path.substr(6);
        }

        // If path starts with int://, append internal resource root
        if (path.substr(0, 6) == "int://")
        {
            return engineResourceRoot + path.substr(6);
        }

        LOG_ERROR("Unresolved path {}", path);
        return path;
    }
}
