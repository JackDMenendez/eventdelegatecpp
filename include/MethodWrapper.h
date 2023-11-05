#ifndef METHODWRAPPER_H
#define METHODWRAPPER_H
#include "IntermediateMemberWrapper.h"
#include "ed_base.h"
EDCPP_BEGIN
template <typename RC, typename Class, typename... Args>
auto MethodWrapper(Class& object, RC (Class::*method)(Args...)) noexcept
    -> IntermediateMemberWrapperExNoCoNoVo<decltype(method),
                                           RC,
                                           Class,
                                           Args...> {
  static_assert(std::is_member_function_pointer<decltype(method)>::value,
                "Must be non-static member function.");
  return IntermediateMemberWrapperExNoCoNoVo<decltype(method), RC, Class,
                                             Args...>(&object, method);
}
template <typename RC, typename Class, typename... Args>
auto MethodWrapper(Class& object,
                   RC (Class::*method)(Args...) noexcept) noexcept
    -> IntermediateMemberWrapperNoExNoCoNoVo<decltype(method),
                                             RC,
                                             Class,
                                             Args...> {
  static_assert(std::is_member_function_pointer<decltype(method)>::value,
                "Must be non-static member function.");
  return IntermediateMemberWrapperNoExNoCoNoVo<decltype(method), RC, Class,
                                               Args...>(&object, method);
}
template <typename RC, typename Class, typename... Args>
auto MethodWrapper(const Class& object,
                   RC (Class::*method)(Args...) const) noexcept
    -> IntermediateMemberWrapperExCoNoVo<decltype(method), RC, Class, Args...> {
  static_assert(std::is_member_function_pointer<decltype(method)>::value,
                "Must be non-static member function.");
  return IntermediateMemberWrapperExCoNoVo<decltype(method), RC, Class,
                                           Args...>(&object, method);
}
template <typename RC, typename Class, typename... Args>
auto MethodWrapper(const Class& object,
                   RC (Class::*method)(Args...) const noexcept) noexcept
    -> IntermediateMemberWrapperNoExCoNoVo<decltype(method),
                                           RC,
                                           Class,
                                           Args...> {
  static_assert(std::is_member_function_pointer<decltype(method)>::value,
                "Must be non-static member function.");
  return IntermediateMemberWrapperNoExCoNoVo<decltype(method), RC, Class,
                                             Args...>(&object, method);
}
EDCPP_END
#endif
