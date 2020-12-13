#include <lotus/filesystem.h>
#include "doctest.h"

TEST_CASE("Test filesystem utilities")
{
    Lotus::SetProjectResourceRoot("path-prefix/");
    SUBCASE("SetProjectResourceRoot")
    {
        CHECK(Lotus::GetProjectResourceRoot() == "path-prefix/");
    }

    SUBCASE("Test ExpandPath")
    {
        CHECK(Lotus::ExpandPath("res://project-resource.json") == "path-prefix/project-resource.json");
        CHECK(Lotus::ExpandPath("int://internal-resource.json") == (Lotus::GetEngineResourceRoot() + "internal-resource.json"));
    }
}
