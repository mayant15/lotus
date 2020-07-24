#include "lotus/debug.h"
#include "lotus/resources.h"

namespace Lotus::Resource
{
    AssetManager& AssetManager::get()
    {
        static AssetManager instance;
        return instance;
    }

    void AssetManager::registerAsset(const std::string& path) {
        LOG_INFO("Registering {}", path);
    }

    template<typename T>
    void AssetManager::registerAsset(T asset)
    {
        LOG_INFO("Registering asset!");
    }

    bool AssetManager::exists(const std::string& path)
    {
        return false;
    }
}