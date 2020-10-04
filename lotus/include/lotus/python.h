#include "lotus/lotus_export.h"

namespace Lotus
{
    /**
     * @brief Executes code in a python file
     * @param filepath Absolute path to the file that is to be executed
    */
    LOTUS_API void exec_file(const std::string& filepath);

    struct CScript
    {
        std::string Path;
    };
}
