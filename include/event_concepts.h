#ifndef EVENT_CONCEPTS_H
#define EVENT_CONCEPTS_H
#include <type_traits>
#include "ed_base.h"
template <typename T>
concept BasicFunction =
    std::is_function<typename std::remove_reference<T>::type>::value;

template <typename Class>
concept InstantiatedMethod =
    !std::is_invocable<typename std::remove_reference<Class>::type>::value;

template <typename T>
concept MemberFunction = std::is_member_function_pointer<T>::value;

template <typename T>
concept NotMemberFunction = !std::is_member_function_pointer<T>::value;

template <typename T, typename... Args>
concept LambdaFunction =
    (!std::is_function<typename std::remove_reference<T>::type>::value) &&
    std::is_invocable<typename std::remove_reference<T>::type, Args...>::value;

template <class fn, class... Args>
concept ThrowInvocable = !std::is_nothrow_invocable<fn, Args...>::value;

template <class fn, class... Args>
concept NothrowInvocable = std::is_nothrow_invocable<fn, Args...>::value;
#endif
