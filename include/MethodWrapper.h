#ifndef METHODWRAPPER_H
#define METHODWRAPPER_H
#include "ed_base.h"
#include "IntermediateMemberWrapper.h"
namespace EDCPP {
template <typename RC, typename Class, typename... Args>
auto MethodWrapper(Class& object, RC (Class::*method)(Args...)) noexcept
    -> IntermediateMemberWrapper<decltype(method), RC, Class, Args...> {
  return IntermediateMemberWrapper<decltype(method), RC, Class, Args...>(
      &object, method);
}
template <typename RC, typename Class, typename... Args>
auto MethodWrapper(const Class& object,
                   RC (Class::*method)(Args...) const) noexcept
    -> IntermediateMemberWrapper<decltype(method), RC, Class, Args...> {
  return IntermediateMemberWrapper<decltype(method), RC, Class, Args...>(
      &object, method);
}
template <typename RC, typename Class, typename... Args>
auto MethodWrapper(const Class& object,
                   RC (Class::*method)(Args...) volatile) noexcept
    -> IntermediateMemberWrapper<decltype(method), RC, Class, Args...> {
  return IntermediateMemberWrapper<decltype(method), RC, Class, Args...>(
      &object, method);
}
}  // namespace EDCPP
#endif
