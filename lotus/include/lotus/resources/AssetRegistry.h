#pragma once

#include <lotus/lcommon.h>

/** @brief Concatenate two strings */
#define CONCAT(x, y) x ## y

/** @brief Macro to conveniently declare asset loaders */
#define LOADER(x) struct LOTUS_API CONCAT(x, Loader) final: public entt::resource_loader<CONCAT(x, Loader), x>

namespace Lotus
{
    /**
     * @brief Get the active instance of the resource cache
     *
     * A resource cache is an EnTT resource cache initialized in the engine dll.
     * This function returns a reference to that cache instance. Each resource has
     * a corresponding template specialization in AssetRegistry.cpp
     *
     * @tparam T Type of the resource to fetch the cache for
     * @return A reference to the cache instance
     */
    template<typename T>
    entt::resource_cache<T>& GetAssetCache();

    /**
     * @brief Load an asset from the given path, or provide the existing asset if already loaded
     * @tparam Resource Type of the resource to load
     * @tparam Loader   Type of the resource loader to use
     * @tparam Args     Types of arguments to be passed to the resource loader
     * @param path      Path of the resource to be loaded
     * @param args      Arguments to be passed to the resource loader
     * @return Handle to the loaded resource instance from the resource cache
     */
    template<typename Resource, typename Loader, typename... Args>
    inline Handle <Resource> LoadAsset(const std::string& path, Args&& ...args)
    {
        auto id = entt::hashed_string::value(path.c_str());
        return GetAssetCache<Resource>().template load<Loader>(id, path, std::forward<Args>(args)...);
    }

    /**
     * @brief Execute a function for each asset in the resource cache
     * @tparam Resource Type of the resource that is under consideration
     * @tparam Func     Type of the function pointer to execute
     * @param func      Function to execute
     */
    template<typename Resource, typename Func>
    inline void ForEachAsset(Func func)
    {
        GetAssetCache<Resource>().each(func);
    }
}
