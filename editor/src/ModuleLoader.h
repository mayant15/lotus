#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <string>

// TODO: Move this to the engine or a completely different library in common?

#define REGISTER_EVENTS_FUNCTION_NAME "RegisterEvents"
#define MODULE_TEST_FUNCTION_NAME "ModuleTestFunction"

namespace Editor
{
#ifdef _WIN32
    using ModuleHandle = HINSTANCE;
#else
    using ModuleHandle = void*;
#endif

    typedef void(*RegisterEventFn)();
    typedef void(*ModuleTestFn)();

    ModuleHandle LoadModule(const std::string& path);
}
