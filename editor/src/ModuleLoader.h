#pragma once

#include <windows.h>
#include <string>

// TODO: Move this to the engine or a completely different library in common?

#define REGISTER_EVENTS_FUNCTION_NAME "RegisterEvents"
#define MODULE_TEST_FUNCTION_NAME "ModuleTestFunction"

namespace Editor
{
    using ModuleHandle = HINSTANCE;
    typedef void(*RegisterEventFn)();
    typedef void(*ModuleTestFn)();

    ModuleHandle LoadModule(const std::string& path);
}
