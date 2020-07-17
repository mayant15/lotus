#include <filesystem>
#include "lotus/engine.h"
#include "lotus/debug.h"

void lotus::init() {
    LOG_INFO("Initializing lotus...");
}

std::string lotus::resource(const std::string& root, const std::string& relPath)
{
    // TODO: This is VERY temporary
    std::filesystem::path path = root;
    return path.parent_path().string() + "/resources/" + relPath;
}
