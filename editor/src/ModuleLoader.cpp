#include "ModuleLoader.h"
#include <stdexcept>

#define REGISTER_EVENTS_HANDLER_NAME "SetupGameEvents"

namespace Editor
{
    register_events_t getRegisterEventsHandler(ModuleHandle module)
    {
        auto handle = (register_events_t) GetProcAddress(module, REGISTER_EVENTS_HANDLER_NAME);
        if (!handle)
        {
            throw std::runtime_error { "Failed to register module functions" };
        }

        return handle;
    }

    ModuleHandle LoadModule(const std::string& path)
    {
        ModuleHandle module = LoadLibrary(path.c_str());
        if (!module)
        {
            throw std::runtime_error { "Failed to load module" };
        }

        auto registerEvents = getRegisterEventsHandler(module);
        registerEvents();

        return module;
    }
}
