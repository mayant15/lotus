#pragma once

#include "Model.h"
#include "Cubemap.h"
#include "Shader.h"

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
