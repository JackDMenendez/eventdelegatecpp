#ifndef EVENT_H
#define EVENT_H
#include <algorithm>
#include <memory>
#include <vector>
#include "CallBackWrapper.h"
#include "ICallBackWrapper.h"
#include "ed_base.h"
namespace EDCPP {
template <typename Signature>
struct Event;

template <typename... Args>
struct Event<void(Args...)> {
  std::vector<std::unique_ptr<ICallBackWrapper<void(Args...)>>> d_callbacks;

  Event(Event const&) = delete;
  void operator=(Event const&) = delete;

 public:
  Event() {}
  template <typename T>
  auto operator+=(T&& callback) -> Event& {
    this->d_callbacks.emplace_back(
        new CallBackWrapper<T, void(Args...)>(std::forward<T>(callback)));
    return *this;
  }
  template <typename T>
  auto operator-=(T&& callback) -> Event& {
    CallBackWrapper<T, void(Args...)> tmp(std::forward<T>(callback));
    auto it = std::remove_if(
        this->d_callbacks.begin(), this->d_callbacks.end(),
        [&](std::unique_ptr<ICallBackWrapper<void(Args...)>>& other) {
          return tmp.do_cmp(other.get());
        });
    this->d_callbacks.erase(it, this->d_callbacks.end());
    return *this;
  }

  void operator()(Args... args) {
    for (auto& callback : this->d_callbacks) {
      callback->do_call(args...);
    }
  }
};
}  // namespace EDCPP
#endif
