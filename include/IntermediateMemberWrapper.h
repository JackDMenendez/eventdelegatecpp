#ifndef INTERMEDIATEMEMBERWRAPPER_H
#define INTERMEDIATEMEMBERWRAPPER_H
#include <iostream>
#include <utility>
#include "ed_base.h"
namespace EDCPP {
template <typename FN, typename RC, typename Class, typename... Args>
class IntermediateMemberWrapper {
  union {
    Class* d_object;
    const Class* c_object;
    volatile Class* v_object;
  };
  union {
    RC (Class::*d_member)(Args...);
    RC (Class::*c_member)(Args...) const;
    RC (Class::*v_member)(Args...) volatile;
  };
// The following tests for the presence util/ghelper.h
#ifdef MTESTREGIME
 public:
  // These are for testing
  static constexpr bool m_const = false;
  static constexpr bool m_volatile = false;
  // m_exception can't be static since method type is orthogonal to the
  // exception regime.
  const bool m_exception = true;
#endif

 public:
  IntermediateMemberWrapper(const Class* object,
                            RC (Class::*method)(Args...) const) noexcept
      : c_object(object), c_member(method) {}
  IntermediateMemberWrapper(const Class* object,
                            RC (Class::*method)(Args...) volatile) noexcept
      : v_object(object), v_member(method) {}
  IntermediateMemberWrapper(Class* object,
                            RC (Class::*method)(Args...)) noexcept
      : d_object(object), d_member(method) {}
  auto operator()(Args... args) -> RC {
    return (this->d_object->*this->d_member)(std::forward<Args>(args)...);
  }
  auto operator()(Args... args) const -> RC {
    return (this->c_object->*this->c_member)(std::forward<Args>(args)...);
  }
  auto operator()(Args... args) volatile -> RC {
    return (this->v_object->*this->v_member)(std::forward<Args>(args)...);
  }
  ~IntermediateMemberWrapper() noexcept {
    std::cout << "~IntermediateMemberWrapper()\n";
  }
};

}
#endif
