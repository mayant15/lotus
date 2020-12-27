#include "doctest.h"

#include <lotus/filesystem.h>
#include <lotus/Config.h>

TEST_CASE("Test filesystem utilities")
{
    Lotus::LoadConfig(PROJECT_SETTINGS);
    SUBCASE("Test ExpandPath")
    {
        CHECK(Lotus::ExpandPath("res://project-resource.json") == "path-prefix/project-resource.json");
    }
}
