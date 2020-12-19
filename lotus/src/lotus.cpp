#include <lotus/entry.h>
#include "core/Engine.h"

namespace Lotus
{
    void Initialize()
    {
        GET(Engine).Initialize();
    }

    void Run()
    {
        GET(Engine).Run();
    }
}
