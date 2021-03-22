#include <lotus/ecs/ComponentRegistry.h>

namespace Lotus
{
    std::unordered_map<component_ctor_key_t, component_ctor_t> ctors;
}
