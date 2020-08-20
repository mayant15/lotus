#pragma once

#include "lotus/physics.h"
#include "lotus/lcommon.h"

namespace Lotus
{
    template<typename T>
    using Handle = entt::resource_handle<T>;

    template<typename T>
    class Cache : public Singleton<Cache<T>>
    {
        entt::resource_cache<T> _cache;

        friend Singleton<Cache<T>>;
        Cache() = default;
    public:
        size_t Size() const
        {
            return _cache.size();
        }

        bool IsEmpty() const
        {
            return _cache.empty();
        }

        // TODO: id_type?
        bool Contains(entt::id_type id) const
        {
            return _cache.contains(id);
        }

        void Clear()
        {
            return _cache.clear();
        }

        template<typename Loader, typename... Args>
        Handle<T> Load(entt::id_type id, Args&& ...args)
        {
            return _cache.template load<Loader>(id, std::forward<Args>(args)...);
        }

        template<typename Loader, typename... Args>
        Handle<T> Reload(Args&& ...args)
        {
            return _cache.template reload<Loader>(std::forward<Args>(args)...);
        }
    };
}
