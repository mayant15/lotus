#pragma once

#if defined(_MSC_VER)
#define PONG_API extern "C" __declspec(dllexport)
#elif defined(__GNUC__)
#define PONG_API extern "C" __attribute__((visibility("default")))
#else
#define PONG_API
#endif
