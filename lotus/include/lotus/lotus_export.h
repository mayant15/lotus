#pragma once

#if defined(_WIN32) || defined(_WIN64)

#ifdef LOTUS_STATIC_DEFINE
#  define LOTUS_API
#  define LOTUS_NO_EXPORT
#else
#  ifndef LOTUS_API
#    ifdef lotus_EXPORTS
/* We are building this library */
#      define LOTUS_API __declspec(dllexport)
#    else
/* We are using this library */
#      define LOTUS_API __declspec(dllimport)
#    endif
#  endif

#  ifndef LOTUS_NO_EXPORT
#    define LOTUS_NO_EXPORT 
#  endif
#endif

#ifndef LOTUS_DEPRECATED
#  define LOTUS_DEPRECATED __declspec(deprecated)
#endif

#ifndef LOTUS_DEPRECATED_EXPORT
#  define LOTUS_DEPRECATED_EXPORT LOTUS_API LOTUS_DEPRECATED
#endif

#ifndef LOTUS_DEPRECATED_NO_EXPORT
#  define LOTUS_DEPRECATED_NO_EXPORT LOTUS_NO_EXPORT LOTUS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LOTUS_NO_DEPRECATED
#    define LOTUS_NO_DEPRECATED
#  endif
#endif

#else
#define LOTUS_API // If we're not on Windows this should expand to nothing
#endif