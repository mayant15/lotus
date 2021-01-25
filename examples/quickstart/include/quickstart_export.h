#pragma once

#if defined(_WIN32) || defined(_WIN64)

#ifdef QUICKSTART_STATIC_DEFINE
#  define QUICKSTART_API
#  define QUICKSTART_NO_EXPORT
#else
#  ifndef QUICKSTART_API
#    ifdef QUICKSTART_EXPORTS
/* We are building this library */
#      define QUICKSTART_API __declspec(dllexport)
#    else
/* We are using this library */
#      define QUICKSTART_API __declspec(dllimport)
#    endif
#  endif

#  ifndef QUICKSTART_NO_EXPORT
#    define QUICKSTART_NO_EXPORT
#  endif
#endif

#ifndef QUICKSTART_DEPRECATED
#  define QUICKSTART_DEPRECATED __declspec(deprecated)
#endif

#ifndef QUICKSTART_DEPRECATED_EXPORT
#  define QUICKSTART_DEPRECATED_EXPORT QUICKSTART_API QUICKSTART_DEPRECATED
#endif

#ifndef QUICKSTART_DEPRECATED_NO_EXPORT
#  define QUICKSTART_DEPRECATED_NO_EXPORT QUICKSTART_NO_EXPORT QUICKSTART_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef QUICKSTART_NO_DEPRECATED
#    define QUICKSTART_NO_DEPRECATED
#  endif
#endif

#else
#define QUICKSTART_API // If we're not on Windows this should expand to nothing
#endif
