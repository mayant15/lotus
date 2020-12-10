#pragma once

#include "lotus/Config.h"

#include <filesystem>

#ifdef SHADER_ROOT

#define VERTEX_PATH(x)   concatPath(SHADER_ROOT, x ".vert")
#define FRAGMENT_PATH(x) concatPath(SHADER_ROOT, x ".frag")

#define SHADER_PBR_VERT  VERTEX_PATH("pbr")
#define SHADER_PBR_FRAG  FRAGMENT_PATH("pbr")
#define SHADER_PBR SHADER_PBR_VERT, SHADER_PBR_FRAG

// TODO: Internal shaders are only used by the engine. Move this definition somewhere inside
#define INTERNAL_SHADERS(x) VERTEX_PATH(x), FRAGMENT_PATH(x)

#else

#error No shader root configured.

#endif

#define RESOURCE(x) getConfigResourcePath(x)

#define CONCAT(x, y) x ## y
#define LOADER(x) struct LOTUS_API CONCAT(x, Loader) final: public entt::resource_loader<CONCAT(x, Loader), x>

inline std::string concatPath(const std::string& path1, const std::string& path2)
{
    std::filesystem::path base(path1);
    return base.append(path2).string();
}

inline std::string getConfigResourcePath(const std::string& relPath)
{
    std::filesystem::path arg = relPath;
    if (arg.is_absolute())
    {
        return relPath;
    }

    std::filesystem::path root =  GET(Lotus::Config).ResourceRoot;
    return root.append(relPath).string();
}

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
}
