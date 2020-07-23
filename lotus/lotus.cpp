#include "lotus/core.h"
#include "lotus/debug.h"

class Engine
{
    Engine() = default;

public:
    static Engine& Get()
    {
        static Engine instance;
        return instance;
    }

    void init()
    {}

    void run()
    {}

    void shutdown()
    {}
};

void Lotus::init()
{
    LOG_INFO("Initializing lotus...");
    Engine& engine = Engine::Get();
    engine.init();
}

void Lotus::run()
{
    Engine& engine = Engine::Get();
    engine.run();
}

void Lotus::shutdown()
{
    Engine& engine = Engine::Get();
    engine.shutdown();
}

//std::string lotus::resource(const std::string& root, const std::string& relPath)
//{
//    // TODO: This is VERY temporary
//    std::filesystem::path path = root;
//    return path.parent_path().string() + "/resources/" + relPath;
//}
