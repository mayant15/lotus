#include "lotus/entry.h"
#include "core/Engine.h"

namespace Lotus
{
    void Initialize(const Config& config)
    {
        GET(Engine).Initialize(config);
    }

    void Run()
    {
        GET(Engine).Run();
    }
}
