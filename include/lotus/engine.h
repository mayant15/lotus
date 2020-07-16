#include <string>

enum class Context {
    OpenGL,
    Vulkan
};
namespace lotus {
    void init();
    std::string resource(const std::string& root, const std::string& relPath);
}
