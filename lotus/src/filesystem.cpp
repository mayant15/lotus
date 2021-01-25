#include <lotus/filesystem.h>
#include <lotus/Config.h>
#include <lotus/debug.h>

#include <filesystem>

namespace Lotus
{
    namespace fs = std::filesystem;

    static const std::string engineResourceRoot = ENGINE_RESOURCE_ROOT;

    static std::string concatPath(fs::path p1, fs::path p2)
    {
        return (p1.make_preferred() / p2.make_preferred()).string();
    }

    static std::string concatPath(const std::string& s1, const std::string& s2)
    {
        return concatPath(fs::path { s1 }, fs::path { s2 });
    }

    std::string ExpandPath(const std::string& path)
    {
        const ProjectConfig& conf = GetProjectConfig();
        std::filesystem::path arg = path;
        if (arg.is_absolute())
        {
            return path;
        }

        // If path starts with res://, append user resource root
        if (path.substr(0, 6) == "res://")
        {
            return concatPath(conf.ProjectResourceRoot, fs::path { path.substr(6) });
        }

        // If path starts with int://, append internal resource root
        if (path.substr(0, 6) == "int://")
        {
            return concatPath(engineResourceRoot, path.substr(6));
        }

        LOG_ERROR("Unresolved path {}", path);
        return path;
    }
}
