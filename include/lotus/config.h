/**
 * Configuration options for lotus, defined as simple cpp macros
 */

#pragma once

// TODO: Dafuq? This is not great at all for users of this library.
#define RENDER_API OPEN_GL

namespace Lotus
{
    enum class Context {
        OPEN_GL,
        DIRECTX,
        VULKAN
    };
}

