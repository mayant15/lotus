#include "ModuleLoader.h"
#include <stdexcept>

namespace Editor
{
    template<typename Fn>
    Fn getFunction(ModuleHandle module, const std::string& name)
    {
        auto handle = (Fn) GetProcAddress(module, name.c_str());
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

        auto testFunction = getFunction<ModuleTestFn>(module, MODULE_TEST_FUNCTION_NAME);
        testFunction();

        auto registerEvents = getFunction<RegisterEventFn>(module, REGISTER_EVENTS_FUNCTION_NAME);
        registerEvents();

        return module;
    }
}
