#ifndef EVENTDELEGATECPP_ED_DEFS_H
#define EVENTDELEGATECPP_ED_DEFS_H

#if defined(__MINGW32__)
#    define EDCPP_MINGW_OS
#elif defined(_WIN32)
#    define EDCPP_WIN_OS
#elif defined(unix) || defined(__unix__) || defined(__unix)
#    define EDCPP_UNIX_OS
#elif defined(__APPLE__) || defined(__MACH__)
#    define EDCPP_MAC_OS
#elif defined(__FreeBSD__)
#    define EDCPP_FREE_BSD_OS
#elif defined(__ANDROID__)
#    define EDCPP_ANDROID_OS
#endif

#if defined(__clang__)
#    define EDCPP_CLANG_COMPILER
#elif defined(__GNUC__) || defined(__GNUG__)
#    define EDCPP_GCC_COMPILER
#elif defined(_MSC_VER)
#    define EDCPP_MSVC_COMPILER
#endif

#if !defined(NDEBUG) || defined(_DEBUG)
#    define EDCPP_DEBUG_MODE
#endif

#if defined(EDCPP_WIN_OS)
#    if defined(EDCPP_EXPORT_API)
#        define EDCPP_API __declspec(dllexport)
#    elif defined(EDCPP_IMPORT_API)
#        define EDCPP_API __declspec(dllimport)
#    endif
#elif (defined(EDCPP_EXPORT_API) || defined(EDCPP_IMPORT_API)) && __has_cpp_attribute(gnu::visibility)
#    define EDCPP_API __attribute__((visibility("default")))
#endif

#if !defined(EDCPP_API)
#    define EDCPP_API
#endif

#include <exception>

#if defined(_LIBCPP_VERSION)
#    define EDCPP_LIBCPP_LIB
#endif

#endif  // PLATFORM_DEFS_H
