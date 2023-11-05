#ifndef COMPILER_H
#define COMPILER_H
#include "environment.h"
#ifndef EDCPP_COMPILER_VERSION
//   The Microsoft was not to define the compiler version with __cplusplus
//   but the newer versions do meet the spec.
#if __cplusplus == 199711L
#define EDCPP_COMPILER_VERSION _MSVC_LANG
#else
#define EDCPP_COMPILER_VERSION __cplusplus
#endif
#endif
#ifndef EDCPP_CXX17
#if EDCPP_COMPILER_VERSION > 201402L
#define EDCPP_CXX17 1
#else
#define EDCPP_CXX17 0
#endif
#endif  // EDCPP_CXX17

#ifndef EDCPP_CXX20
#if EDCPP_CXX17 && EDCPP_COMPILER_VERSION > 201703L
#define EDCPP_CXX20 1
#else
#define EDCPP_CXX20 0
#endif
#endif  // EDCPP_CXX20

#ifndef EDCPP_CXX23
#if EDCPP_CXX20 && EDCPP_COMPILER_VERSION > 202002L
#define EDCPP_CXX23 1
#else
#define EDCPP_CXX23 0
#endif
#endif  // EDCPP_CXX23

// P2465R3 Standard Library Modules std And std.compat
#if EDCPP_CXX23 && defined(EDCPP_BUILD_MODULE)
#define EDCPP_EXPORT export
#else  // EDCPP_CXX23 && defined(EDCPP_BUILD_MODULE)
#define EDCPP_EXPORT
#endif  // EDCPP_CXX23 && defined(EDCPP_BUILD_MODULE)
#endif
