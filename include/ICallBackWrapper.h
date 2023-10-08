#ifndef ICALLBACKWRAPPER_H
#define ICALLBACKWRAPPER_H
#include <iostream>
#include "ed_base.h"
namespace EDCPP {
template <typename Signature>
struct ICallBackWrapper;

template <typename... Args>
struct ICallBackWrapper<void(Args...)> {
  virtual ~ICallBackWrapper() noexcept { std::cout << "~ICallBackWrapper()\n"; }
  virtual bool do_cmp(ICallBackWrapper* other) const noexcept = 0;
  virtual void do_call_noexcept(Args... args) noexcept = 0;
  virtual void do_call_except(Args... args) = 0;
  virtual uint64_t get_subscriber_id() const noexcept = 0;
  virtual void set_subscriber_id(uint64_t id) noexcept = 0;
  virtual bool throws_exception() const noexcept = 0;
  virtual void set_throws_exception(bool throws_exception_flag) noexcept = 0;
};  // namespace EDCPP
}
#endif
