// TODO: spdlog as a usage dependency
#include <spdlog/spdlog.h>

#define LOG_INFO(...)   spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)   spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...)  spdlog::error(__VA_ARGS__)

//namespace Lotus
//{
//
//    template<typename T>
//    void info(const T& msg);
//
//    template<typename ...Args>
//    void info(const char* fmt, const Args& ...);
//
//    template<typename T>
//    void warn(const T& msg);
//
//    template<typename ...Args>
//    void warn(const char* fmt, const Args& ...);
//
//    template<typename T>
//    void error(const T& msg);
//
//    template<typename ...Args>
//    void error(const char* fmt, const Args& ...);
//}
