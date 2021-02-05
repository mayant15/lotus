#include "doctest.h"

#include <lotus/filesystem.h>
#include <lotus/Config.h>

#include <filesystem>

TEST_CASE("Test filesystem utilities")
{
    Lotus::LoadConfig(PROJECT_SETTINGS);
    SUBCASE("Test ExpandPath")
    {
        std::filesystem::path path { PROJECT_SETTINGS };
        path = path.replace_filename("resources");
        path = path.concat("/project-resource.json").make_preferred();

        CHECK(Lotus::ExpandPath("res://project-resource.json") == path.string());
    }
}
