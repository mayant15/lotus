#include "utils.h"
#include <nfd.h>

namespace Editor
{
    std::optional<std::string> OpenFileDialog()
    {
        nfdchar_t *outPath = nullptr;
        nfdresult_t result = NFD_OpenDialog( nullptr, nullptr, &outPath );

        if ( result == NFD_OKAY ) {
            std::string path (outPath);
            free(outPath);

            return { path };
        }
        else
        {
            return std::nullopt;
        }
    }

    std::string ExpandPath(const std::string& path)
    {
        return EDITOR_RESOURCE_ROOT + path;
    }
}
