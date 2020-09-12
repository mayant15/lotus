#pragma once

#include "Model.h"
#include "Cubemap.h"
#include "Shader.h"
#include "lotus/Config.h"

#include <filesystem>


#ifdef SHADER_ROOT

#define __VERTEX_PATH(x)   __concatPath(SHADER_ROOT, x ".vert")
#define __FRAGMENT_PATH(x) __concatPath(SHADER_ROOT, x ".frag")

#define SHADER_PBR_VERT  __VERTEX_PATH("pbr")
#define SHADER_PBR_FRAG  __FRAGMENT_PATH("pbr")
#define SHADER_PBR SHADER_PBR_VERT, SHADER_PBR_FRAG

#define INTERNAL_SHADERS(x) __VERTEX_PATH(x), __FRAGMENT_PATH(x)

#else

#error No shader root configured.

#endif

#define RESOURCE(x) __getConfigResourcePath(x)

inline std::string __concatPath(const std::string& path1, const std::string& path2)
{
    std::filesystem::path base(path1);
    return base.append(path2).string();
}

inline std::string __getConfigResourcePath(const std::string& relPath)
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
    class LOTUS_API AssetRegistry : public Singleton<AssetRegistry>
    {
        entt::resource_cache<Texture> _textures;
        entt::resource_cache<Model> _models;
        entt::resource_cache<Cubemap> _cubemaps;
        entt::resource_cache<Shader> _shaders;
        entt::resource_cache<Material> _materials;

        uint32_t _currentID;

    public:
        // size_t Size() const
        // {
        //     return _cache.size();
        // }
        //
        // bool IsEmpty() const
        // {
        //     return _cache.empty();
        // }

        // TODO: id_type?
        // bool Contains(entt::id_type id) const
        // {
        //     return _cache.contains(id);
        // }

        // void Clear()
        // {
        //     return _cache.clear();
        // }


        // TODO: Template out everything
        template<typename... Args>
        Handle<Texture> LoadTexture(Args&& ...args)
        {
            auto identifier = entt::hashed_string(std::to_string(_currentID).c_str());
            _currentID++;
            return _textures.load<TextureLoader>(identifier, std::forward<Args>(args)...);
        }

        template<typename... Args>
        Handle<Model> LoadModel(Args&& ...args)
        {
            auto identifier = entt::hashed_string(std::to_string(_currentID).c_str());
            _currentID++;
            return _models.load<ModelLoader>(identifier, std::forward<Args>(args)...);
        }

        template<typename... Args>
        Handle<Cubemap> LoadCubemap(Args&& ...args)
        {
            auto identifier = entt::hashed_string(std::to_string(_currentID).c_str());
            _currentID++;
            return _cubemaps.load<CubemapLoader>(identifier, std::forward<Args>(args)...);
        }

        template<typename... Args>
        Handle<Shader> LoadShader(Args&& ...args)
        {
            auto identifier = entt::hashed_string(std::to_string(_currentID).c_str());
            _currentID++;
            return _shaders.load<ShaderLoader>(identifier, std::forward<Args>(args)...);
        }

        template<typename... Args>
        Handle<Material> LoadMaterial(Args&& ...args)
        {
            auto identifier = entt::hashed_string(std::to_string(_currentID).c_str());
            _currentID++;
            return _materials.load<MaterialLoader>(identifier, std::forward<Args>(args)...);
        }

        // template<typename Loader, typename... Args>
        // Handle<T> Reload(Args&& ...args)
        // {
        //     return _cache.template reload<Loader>(std::forward<Args>(args)...);
        // }
    };
}
