#pragma once

#define CONCAT(x, y) x ## y
#define LOADER(x) struct LOTUS_API CONCAT(x, Loader) final: public entt::resource_loader<CONCAT(x, Loader), x>

namespace Lotus
{
    template <typename T>
    struct AssetCache { static inline entt::resource_cache<T> cache {}; };

    template <typename Resource, typename Loader, typename... Args>
    inline Handle<Resource> LoadAsset(const std::string& path, Args&& ...args)
    {
        auto id = entt::hashed_string::value(path.c_str());
        return AssetCache<Resource>::cache.template load<Loader>(id, path, std::forward<Args>(args)...);
    }

    template <typename Resource, typename Func>
    inline void ForEachAsset(Func func)
    {
        AssetCache<Resource>::cache.each(func);
    }
}
