#ifndef MESSAGES_H
#define MESSAGES_H
#define EDCPP_PRAGMA_MESSAGE(MESSAGE) _STL_PRAGMA(message(MESSAGE))
#define EDCPP_EMIT_MESSAGE(MESSAGE)   _STL_PRAGMA_MESSAGE(__FILE__ "(" _CRT_STRINGIZE(__LINE__) "): " MESSAGE)

#define EDCPP_EMIT_STL_WARNING(NUMBER, MESSAGE)             \
    EDCPP_EMIT_MESSAGE("warning " #NUMBER ": " MESSAGE) \
    static_assert(true, "")
#define EDCPP_EMIT_ERROR(NUMBER, MESSAGE)             \
    EDCPP_EMIT_MESSAGE("error " #NUMBER ": " MESSAGE) \
    static_assert(false, "Error in EDCPP usage.")
#endif
