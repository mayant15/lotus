#include "doctest.h"

#include <lotus/Config.h>

TEST_CASE ("Test Config")
{
    Lotus::LoadConfig(PROJECT_SETTINGS);

    SUBCASE("Test GetProjectConfig")
    {
        std::filesystem::path path { PROJECT_SETTINGS };
        path = path.replace_filename("resources").make_preferred();

        auto prConf = Lotus::GetProjectConfig();
        CHECK(prConf.ProjectResourceRoot == path);
    }

    SUBCASE("Test GetBuildConfig")
    {
        auto bConf = Lotus::GetBuildConfig();
        CHECK(bConf.IsDebug == true);
    }

    SUBCASE("Test GetRenderConfig")
    {
        auto rConf = Lotus::GetRenderConfig();
        CHECK(rConf.RenderAPI == Lotus::ERenderAPI::OPEN_GL);
        CHECK(rConf.ViewportWidth == 1024);
        CHECK(rConf.ViewportHeight == 800);
        CHECK(rConf.DepthTest == true);
        CHECK(rConf.CullFace == true);
        CHECK(rConf.MSAA == true);
    }
}
