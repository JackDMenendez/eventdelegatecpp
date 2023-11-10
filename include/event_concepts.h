#ifndef EVENT_CONCEPTS_H
#define EVENT_CONCEPTS_H
#include <type_traits>
#include "ed_base.h"
#include "ed_traits.h"

EDCPP_BEGIN
template <typename T>
concept RCVoid = _STD is_same<T, void>::value;
template <typename T> 
concept RCNotVoid = ! _STD is_same<T, void>::value;

template <class I>
concept DefaultDelegateFunction = is_default_delegate_v<I>;
/**
 * @brief Used to identify functions that are not lambdas and not member
 * functions.
 *
 * @details
 * Checks whether FN is a function type. Types like std::function, lambdas,
 * classes with overloaded operator() and pointers to functions don't count as
 * function types.
 *
 * Exclude when FN is a non-static member function pointer but allows static
 * member functions.
 *
 * @tparam FN A function type.
 */
template <typename FN>
concept BasicFunction =
    std::is_function<FN>::value && !std::is_member_function_pointer<FN>::value;

template <typename Class>
concept InstantiatedMethod =
    !std::is_invocable<typename std::remove_reference<Class>::type>::value;

template <typename T>
concept MemberFunction = std::is_member_function_pointer<T>::value;

template <typename FN>
concept ConstFunction = std::is_const<FN>::value;

template <typename FN>
concept VolatileFunction = std::is_volatile<FN>::value;

template <typename FN>
concept NotVolatileFunction = !std::is_volatile<FN>::value;

template <typename FN>
concept NotConstFunction = !std::is_const<FN>::value;
template <typename T>
concept NotMemberFunction = !std::is_member_function_pointer<T>::value;

template <typename T, typename... Args>
concept LambdaMemberFunction =
    (!std::is_function<typename std::remove_reference<T>::type>::value) &&
    (std::is_member_function_pointer<T>::value) &&
    std::is_invocable<typename std::remove_reference<T>::type, Args...>::value;

template <typename T, typename... Args>
concept LambdaFunction =
    (!std::is_function<typename std::remove_reference<T>::type>::value) &&
    (!std::is_member_function_pointer<T>::value) &&
    std::is_invocable<typename std::remove_reference<T>::type, Args...>::value;

template <class fn, class... Args>
concept ThrowInvocable = !std::is_nothrow_invocable<fn, Args...>::value;

template <class fn, class... Args>
concept NoThrowInvocable = std::is_nothrow_invocable<fn, Args...>::value;
EDCPP_END
#endif
