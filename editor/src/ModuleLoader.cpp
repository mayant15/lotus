#include "ModuleLoader.h"
#include <stdexcept>

namespace Editor
{
    template<typename Fn>
    Fn getFunction(ModuleHandle module, const std::string& name)
    {
#ifdef _WIN32
        auto handle = (Fn) GetProcAddress(module, name.c_str());
#else
        auto handle = (Fn) dlsym(module, name.c_str());
#endif

        if (!handle)
        {
            throw std::runtime_error { "Failed to register module functions" };
        }

        return handle;
    }

    ModuleHandle LoadModule(const std::string& path)
    {
#ifdef _WIN32
        ModuleHandle module = LoadLibrary(path.c_str());
#else
        ModuleHandle module = dlopen(path.c_str(), RTLD_LAZY);
#endif
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
