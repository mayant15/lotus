#include <iostream>
#include <filesystem>
#include "lotus/engine.h"

void lotus::init() {
    std::cout << "Initializing lotus" << std::endl;
}

std::string lotus::resource(const std::string& root, const std::string& relPath)
{
    // TODO: This is VERY temporary
    std::filesystem::path path = root;
    return path.parent_path().string() + "/resources/" + relPath;
}
