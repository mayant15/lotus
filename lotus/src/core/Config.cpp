#include <lotus/Config.h>
#include <lotus/debug.h>

#include <fstream>

namespace Lotus
{
    static ProjectConfig projectConfig {};
    static BuildConfig buildConfig {};
    static RenderConfig renderConfig {};

    void LoadConfig(const std::string& path)
    {
        using namespace nlohmann;
        std::ifstream file (path);
        json data;
        file >> data;

        try
        {
            projectConfig = data.at("Project").get<ProjectConfig>();
            buildConfig = data.at("Build").get<BuildConfig>();
            renderConfig = data.at("Render").get<RenderConfig>();
        }
        catch (const std::exception& e)
        {
            LOG_ERROR(e.what());
        }
    }

    const ProjectConfig& GetProjectConfig()
    {
        return projectConfig;
    }

    const RenderConfig& GetRenderConfig()
    {
        return renderConfig;
    }

    const BuildConfig& GetBuildConfig()
    {
        return buildConfig;
    }
}
