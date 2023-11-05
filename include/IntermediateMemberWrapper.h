#ifndef INTERMEDIATEMEMBERWRAPPER_H
#define INTERMEDIATEMEMBERWRAPPER_H
#include <iostream>
#include <type_traits>
#include <utility>
#include "ed_base.h"
#include "event_concepts.h"
EDCPP_BEGIN
// TODO use CPP insights to parse the FN type and specialize this template
template <typename FN, typename RC, typename Class, typename... Args>
class IntermediateMemberWrapperExNoCoNoVo {
  Class* d_object;
  RC (Class::*d_member)(Args...);

 public:
  using FunctionType = FN;
#ifdef MTESTREGIME
  // These are for testing
  // bool m_const;
  bool m_const;
  // bool m_volatile;
  bool m_volatile;

  // m_exception can't be static since method type is orthogonal to the
  // exception regime.
  // bool m_noexcept;
  bool m_noexcept;
#endif
  IntermediateMemberWrapperExNoCoNoVo& operator=(
      const IntermediateMemberWrapperExNoCoNoVo& object) noexcept {
    d_object = object.d_object;
    d_member = object.d_member;
#ifdef MTESTREGIME
    m_const = object.m_const;
    m_volatile = object.m_volatile;
    m_noexcept = object.m_noexcept;
#endif
  }
  IntermediateMemberWrapperExNoCoNoVo(
      const IntermediateMemberWrapperExNoCoNoVo& object) noexcept {
    d_object = object.d_object;
    d_member = object.d_member;
#ifdef MTESTREGIME
    m_const = object.m_const;
    m_volatile = object.m_volatile;
    m_noexcept = object.m_noexcept;
#endif
  }
  IntermediateMemberWrapperExNoCoNoVo(Class* object,
                                      RC (Class::*method)(Args...)) noexcept
      : d_object(object),
        d_member(method)
#ifdef MTESTREGIME
        ,
        m_const(false),
        m_volatile(false),
        m_noexcept(false)
#endif
  {
  }
  auto operator()(Args... args) -> RC {
#ifdef MTESTREGIME
    assert(!m_const);
    assert(!m_volatile);
    assert(!m_noexcept);
#endif
    return (this->d_object->*this->d_member)(std::forward<Args>(args)...);
  }
  ~IntermediateMemberWrapperExNoCoNoVo() noexcept {
    // std::cout << "~IntermediateMemberWrapperExNoCoNoVo()\n";
  }
};
template <typename FN, typename RC, typename Class, typename... Args>
class IntermediateMemberWrapperNoExNoCoNoVo {
  Class* d_object;
  RC (Class::*dn_member)(Args...) noexcept;

 public:
#ifdef MTESTREGIME
  // These are for testing
  // bool m_const;
  bool m_const;
  // bool m_volatile;
  bool m_volatile;

  // m_exception can't be static since method type is orthogonal to the
  // exception regime.
  // bool m_noexcept;
  bool m_noexcept;
#endif
  IntermediateMemberWrapperNoExNoCoNoVo& operator=(
      const IntermediateMemberWrapperNoExNoCoNoVo& object) noexcept {
    d_object = object.d_object;
    dn_member = object.dn_member;
#ifdef MTESTREGIME
    m_const = object.m_const;
    m_volatile = object.m_volatile;
    m_noexcept = object.m_noexcept;
#endif
  }
  IntermediateMemberWrapperNoExNoCoNoVo(
      const IntermediateMemberWrapperNoExNoCoNoVo& object) noexcept {
    d_object = object.d_object;
    dn_member = object.dn_member;
#ifdef MTESTREGIME
    m_const = object.m_const;
    m_volatile = object.m_volatile;
    m_noexcept = object.m_noexcept;
#endif
  }

  IntermediateMemberWrapperNoExNoCoNoVo(
      Class* object,
      RC (Class::*method)(Args...) noexcept) noexcept
      : d_object(object),
        dn_member(method)
#ifdef MTESTREGIME
        ,
        m_const(false),
        m_volatile(false),
        m_noexcept(true)
#endif
  {
  }
  auto operator()(Args... args) noexcept -> RC {
#ifdef MTESTREGIME
    assert(!m_const);
    assert(!m_volatile);
    assert(m_noexcept);
#endif
    return (this->d_object->*this->dn_member)(std::forward<Args>(args)...);
  }
  ~IntermediateMemberWrapperNoExNoCoNoVo() noexcept {
    // std::cout << "~IntermediateMemberWrapperNoExNoCoNoVo()\n";
  }
};
template <typename FN, typename RC, typename Class, typename... Args>
class IntermediateMemberWrapperExCoNoVo {
  const Class* c_object;
  RC (Class::*c_member)(Args...) const;

 public:
#ifdef MTESTREGIME
  // These are for testing
  // bool m_const;
  bool m_const;
  // bool m_volatile;
  bool m_volatile;

  // m_exception can't be static since method type is orthogonal to the
  // exception regime.
  // bool m_noexcept;
  bool m_noexcept;
#endif
  IntermediateMemberWrapperExCoNoVo& operator=(
      const IntermediateMemberWrapperExCoNoVo& object) noexcept {
    c_object = object.c_object;
    c_member = object.c_member;
#ifdef MTESTREGIME
    m_const = object.m_const;
    m_volatile = object.m_volatile;
    m_noexcept = object.m_noexcept;
#endif
  }
  IntermediateMemberWrapperExCoNoVo(
      const IntermediateMemberWrapperExCoNoVo& object) noexcept {
    c_object = object.c_object;
    c_member = object.c_member;
#ifdef MTESTREGIME
    m_const = object.m_const;
    m_volatile = object.m_volatile;
    m_noexcept = object.m_noexcept;
#endif
  }
  IntermediateMemberWrapperExCoNoVo(const Class* object,
                                    RC (Class::*method)(Args...) const) noexcept
      : c_object(object),
        c_member(method)
#ifdef MTESTREGIME
        ,
        m_const(true),
        m_volatile(false),
        m_noexcept(false)
#endif
  {
  }
  auto operator()(Args... args) const -> RC {
#ifdef MTESTREGIME
    assert(m_const);
    assert(!m_volatile);
    assert(!m_noexcept);
#endif
    return (this->c_object->*this->c_member)(std::forward<Args>(args)...);
  }
  ~IntermediateMemberWrapperExCoNoVo() noexcept {
    // std::cout << "~IntermediateMemberWrapperExCoNoVo()\n";
  }
};
template <typename FN, typename RC, typename Class, typename... Args>
class IntermediateMemberWrapperNoExCoNoVo {
  const Class* c_object;
  RC (Class::*cn_member)(Args...) const noexcept;

 public:
#ifdef MTESTREGIME
  // These are for testing
  // bool m_const;
  bool m_const;
  // bool m_volatile;
  bool m_volatile;

  // m_exception can't be static since method type is orthogonal to the
  // exception regime.
  // bool m_noexcept;
  bool m_noexcept;
#endif
  IntermediateMemberWrapperNoExCoNoVo& operator=(
      const IntermediateMemberWrapperNoExCoNoVo& object) noexcept {
    c_object = object.c_object;
    cn_member = object.cn_member;
#ifdef MTESTREGIME
    m_const = object.m_const;
    m_volatile = object.m_volatile;
    m_noexcept = object.m_noexcept;
#endif
  }
  IntermediateMemberWrapperNoExCoNoVo(
      const IntermediateMemberWrapperNoExCoNoVo& object) noexcept {
    c_object = object.c_object;
    cn_member = object.cn_member;
#ifdef MTESTREGIME
    m_const = object.m_const;
    m_volatile = object.m_volatile;
    m_noexcept = object.m_noexcept;
#endif
  }
  IntermediateMemberWrapperNoExCoNoVo(const Class* object,
                                      RC (Class::*method)(Args...)
                                          const noexcept) noexcept
      : c_object(object),
        cn_member(method)
#ifdef MTESTREGIME
        ,
        m_const(true),
        m_volatile(false),
        m_noexcept(true)
#endif
  {
  }
  auto operator()(Args... args) const noexcept -> RC {
#ifdef MTESTREGIME
    assert(m_const);
    assert(!m_volatile);
    assert(m_noexcept);
#endif
    return (this->c_object->*this->cn_member)(std::forward<Args>(args)...);
  }
  ~IntermediateMemberWrapperNoExCoNoVo() noexcept {
    // std::cout << "~IntermediateMemberWrapperNoExCoNoVo()\n";
  }
};
EDCPP_END
#endif
