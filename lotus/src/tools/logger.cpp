#include <spdlog/spdlog.h>
#include "lotus/debug.h"

namespace Lotus
{
    template<typename T>
    void info(const T& msg)
    {
        spdlog::info(msg);
    }

    template<typename ...Args>
    void info(const char* fmt, const Args& ... args)
    {
        spdlog::info(fmt, args...);
    }

    template<typename T>
    void warn(const T& msg)
    {
        spdlog::warn(msg);
    }

    template<typename ...Args>
    void warn(const char* fmt, const Args& ... args)
    {
        spdlog::warn(fmt, args...);
    }

    template<typename T>
    void error(const T& msg)
    {
        spdlog::error(msg);
    }

    template<typename ...Args>
    void error(const char* fmt, const Args& ... args)
    {
        spdlog::error(fmt, args...);
    }
}
