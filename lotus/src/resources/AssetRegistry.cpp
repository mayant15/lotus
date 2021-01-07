#include <lotus/resources/AssetRegistry.h>

#include <lotus/resources/Shader.h>
#include <lotus/resources/HDRI.h>
#include <lotus/resources/Material.h>
#include <lotus/resources/Model.h>

#define CACHE_INSTANCE(Resource) \
    template<> LOTUS_API entt::resource_cache<Resource>& GetAssetCache<Resource>() { \
        static entt::resource_cache<Resource> cache {}; \
        return cache; \
    }

namespace Lotus
{
    /**
     * Template instantiations for asset caches. These need to be here in this cpp file so that
     * the caches persist across boundaries (between the lotus dll and the user's game).
     */

    CACHE_INSTANCE(Shader);
    CACHE_INSTANCE(HDRI);
    CACHE_INSTANCE(Material);
    CACHE_INSTANCE(Model);
    CACHE_INSTANCE(Texture);

//    template<>
//    inline Handle <Shader> LoadAsset<Shader, ShaderLoader>(const std::string& path, const std::string& args)
//    {
//        auto id = entt::hashed_string::value((path + args).c_str());
//        return GetCache<Shader>().load<ShaderLoader>(id, path, args);
//    }
}
