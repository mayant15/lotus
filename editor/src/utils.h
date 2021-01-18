#pragma once

#include <optional>
#include <string>

namespace Editor
{
    std::optional<std::string> OpenFileDialog();

    // TODO: Move primitives the editor, int://. Shaders?
    std::string ExpandPath(const std::string& path);

    void LoadScene(const std::string& path);
}
