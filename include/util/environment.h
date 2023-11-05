#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#if defined(__MINGW32__)
/** MINGW Tool Chain is in use */
#define EDCPP_MINGW_OS
#elif defined(_WIN32)
/** Windows compile */
#define EDCPP_WIN_OS
#elif defined(unix) || defined(__unix__) || defined(__unix)
/** Unix compile */
#define EDCPP_UNIX_OS
#elif defined(__APPLE__) || defined(__MACH__)
/** MAC OS compile */
#define EDCPP_MAC_OS
#elif defined(__FreeBSD__)
/** Free BSD compile */
#define EDCPP_FREE_BSD_OS
#elif defined(__ANDROID__)
/** Android compile */
#define EDCPP_ANDROID_OS
#endif

#if defined(__clang__)
#define EDCPP_CLANG_COMPILER
#elif defined(__GNUC__) || defined(__GNUG__)
#define EDCPP_GCC_COMPILER
#elif defined(_MSC_VER)
#define EDCPP_MSVC_COMPILER
#endif
#endif
