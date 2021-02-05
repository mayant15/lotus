#include <buildtool.h>

#ifdef _WIN32
#include "windows.h"
#endif

int main(int argv, char** argc)
{
    std::cout << "This is the build tool. Use this to verify the example game DLLs." << std::endl;

#ifdef _WIN32
    HINSTANCE module = LoadLibrary("quickstart.dll");
    auto fn = GetProcAddress(module, "ModuleTestFunction");
    fn();
#endif

    return 0;
}
