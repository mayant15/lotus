#pragma once

#include <windows.h>
#include <string>

namespace Editor
{
    using ModuleHandle = HINSTANCE;
    typedef void(__cdecl *register_events_t)();

    ModuleHandle LoadModule(const std::string& path);
}
