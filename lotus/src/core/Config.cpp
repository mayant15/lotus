#include <lotus/Config.h>
#include <lotus/debug.h>

#include <fstream>

#define RESOURCE_DIR "resources"
#define BUILD_DIR "build"
#define SETTING_FILENAME ".lproject"


namespace Lotus
{
    static ProjectConfig projectConfig {};
    static BuildConfig buildConfig {};
    static RenderConfig renderConfig {};

    void LoadConfig(const std::filesystem::path& path)
    {
        using namespace nlohmann;
        std::ifstream file(path / std::filesystem::path{ SETTING_FILENAME });
        json data;
        file >> data;

        try
        {
            projectConfig = data.at("Project").get<ProjectConfig>();
            projectConfig.ProjectRoot = path;
            projectConfig.ProjectResourceRoot = path / std::filesystem::path { RESOURCE_DIR };
            projectConfig.ProjectBuildRoot = path / std::filesystem::path { BUILD_DIR };

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
