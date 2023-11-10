#ifndef EVENTDELEGATECPP_ED_DEFS_H
#define EVENTDELEGATECPP_ED_DEFS_H
#include "compiler.h"
#include "environment.h"
#if !defined(NDEBUG) || defined(_DEBUG)
#define EDCPP_DEBUG_MODE
#endif
#if defined(EDCPP_WIN_OS)
#if defined(EDCPP_EXPORT_API)
#define EDCPP_API __declspec(dllexport)
#elif defined(EDCPP_IMPORT_API)
#define EDCPP_API __declspec(dllimport)
#endif
#elif (defined(EDCPP_EXPORT_API) || defined(EDCPP_IMPORT_API)) && \
    __has_cpp_attribute(gnu::visibility)
#define EDCPP_API __attribute__((visibility("default")))
#endif
#if !defined(EDCPP_API)
#define EDCPP_API
#endif
#include <exception>
#if defined(_LIBCPP_VERSION)
#define EDCPP_LIBCPP_LIB
#endif
#if defined(__CUDACC__) || defined(__INTEL_COMPILER)
#define EDCPP_PRAGMA(PRAGMA) __pragma(PRAGMA)
#else
#define EDCPP_PRAGMA(PRAGMA) _Pragma(#PRAGMA)
#endif
#if EDCPP_CXX23 && defined(EDCPP_BUILD_MODULE)
#define EDCPP_EXPORT export
#else  // EDCPP_CXX23 && defined(EDCPP_BUILD_MODULE)
#define EDCPP_EXPORT
#endif  // EDCPP_CXX23 && defined(_BUILD_STD_MODULE)
#include "messages.h"
#define _MEDCPP eventdelegatecpp
#define _EDCPP ::_MEDCPP::
#define EDCPP_BEGIN namespace _MEDCPP {
#define EDCPP_END }
#define EDCPP_COMBINE(N) _MEDCPP::N
#define EDCPP_USING using namespace _MEDCPP;
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif
// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif
#ifdef EDCPP_MSVC_COMPILER
#ifdef ENVIRONMENT64
#define EDCPP_CALL __vectorcall
// #define EDCPP_CALL __cdecl
#else
#define EDCPP_CALL __cdecl
#endif
#else
#define EDCPP_CALL
#endif
#endif  // ED_DEFS_H
