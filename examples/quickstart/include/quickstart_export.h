#pragma once

#if defined(_MSC_VER)
#define QUICKSTART_API extern "C" __declspec(dllexport)
#elif defined(__GNUC__)
#define QUICKSTART_API extern "C" __attribute__((visibility("default")))
#else
#define QUICKSTART_API
#endif
