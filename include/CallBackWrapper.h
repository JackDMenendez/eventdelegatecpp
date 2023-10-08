#ifndef CALLBACKWRAPPER_H
#define CALLBACKWRAPPER_H
#include <iostream>
#include "ed_base.h"
#include "ICallBackWrapper.h"
namespace EDCPP {
template <typename T, typename Signature>
struct CallBackWrapper;
// need different one of these for closure style lambdas?
template <typename T, typename... Args>
struct CallBackWrapper<T, void(Args...)> : ICallBackWrapper<void(Args...)> {
  T d_callback;
  bool m_throws_exception = true;
  std::uint64_t subscriber_id;
  template <typename S>
  CallBackWrapper(S&& callback) noexcept
      : d_callback(std::forward<S>(callback)) {
    std::cout << "CallBackWrapper(S&& callback)\n";
  }
  CallBackWrapper(CallBackWrapper&) = delete;
  CallBackWrapper& operator=(CallBackWrapper&) = delete;
  bool do_cmp(ICallBackWrapper<void(Args...)>* other) const noexcept override {
    return this->get_subscriber_id() == other->get_subscriber_id();
  }
  void do_call_except(Args... args) override {
    return this->d_callback(std::forward<Args>(args)...);
  }
  void do_call_noexcept(Args... args) noexcept override {
    return this->d_callback(std::forward<Args>(args)...);
  }
  std::uint64_t get_subscriber_id() const noexcept override {
    return subscriber_id;
  }
  void set_subscriber_id(std::uint64_t id) noexcept override {
    subscriber_id = id;
  }
  bool throws_exception() const noexcept override { return m_throws_exception; }
  void set_throws_exception(bool throws_exception_flag) noexcept override {
    m_throws_exception = throws_exception_flag;
  }
  ~CallBackWrapper() noexcept override { std::cout << "~CallBackWrapper()\n"; }
};
}  // namespace EDCPP
#endif
