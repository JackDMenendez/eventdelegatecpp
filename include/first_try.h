#ifndef FIRST_TRY_H
#define FIRST_TRY_H
#include <atomic>
#include <functional>
#include <iostream>
#include <memory>
#include <print>
#include <vector>
#include "senders/default_sender.h"
namespace ED {
template <typename Signature>
struct ICall;

template <typename... Args>
struct ICall<void(Args...)> {
  virtual ~ICall() noexcept { 
          //std::cout << "~ICall()\n"; 
  }
  virtual auto do_cmp(ICall* other) noexcept -> bool = 0;
  virtual void do_call(Args... args) noexcept = 0;
};  // namespace EDCPP

template <typename T, typename Signature>
struct Call;
// need different one of these for closure style lambdas?
template <typename T, typename... Args>
struct Call<T, void(Args...)> : ICall<void(Args...)> {
  T d_callback;
  template <typename S>
  Call(S&& callback) : d_callback(std::forward<S>(callback)) {}
  Call(Call& call) : d_callback(std::forward<T>(call.d_callback)) {}
  Call& operator=(Call& call) { d_callback = std::forward<T>(call.d_callback); }
  // Call() : d_callback(nullptr) { std::println(std::stdout, "Call()"); }
  auto do_cmp(ICall<void(Args...)>* other) noexcept -> bool {
    Call<T, void(Args...)>* tmp = dynamic_cast<Call<T, void(Args...)>*>(other);
    return tmp && this->d_callback == tmp->d_callback;
  }
  void do_call(Args... args) noexcept {
    return this->d_callback(std::forward<Args>(args)...);
  }
  virtual ~Call() noexcept {
    //        std::cout << "~Call()\n";
  }
};
template <typename RC, typename Class, typename... Args>
class member_call {
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

 public:
  member_call(const Class* object, RC (Class::*member)(Args...) const) noexcept
      : c_object(object), c_member(member) {}
  member_call(const Class* object,
              RC (Class::*member)(Args...) volatile) noexcept
      : v_object(object), v_member(member) {}
  member_call(Class* object, RC (Class::*member)(Args...)) noexcept
      : d_object(object), d_member(member) {}
  auto operator()(Args... args) -> RC {
    return (this->d_object->*this->d_member)(std::forward<Args>(args)...);
  }
  auto operator()(Args... args) const -> RC {
    return (this->c_object->*this->c_member)(std::forward<Args>(args)...);
  }
  auto operator()(Args... args) volatile -> RC {
    return (this->v_object->*this->v_member)(std::forward<Args>(args)...);
  }
  auto operator==(member_call const& other) const noexcept -> bool {
    return this->d_object == other.d_object && this->d_member == other.d_member;
  }
  auto operator!=(member_call const& other) const noexcept -> bool {
    return !(*this == other);
  }
  ~member_call() noexcept {
    //        std::cout << "~member_call()\n";
  }
};
template <typename RC, typename Class, typename... Args>
auto member(Class& object, RC (Class::*member)(Args...))
    -> member_call<RC, Class, Args...> {
  return member_call<RC, Class, Args...>(&object, member);
}
template <typename RC, typename Class, typename... Args>
auto member(const Class& object, RC (Class::*member)(Args...) const)
    -> member_call<RC, Class, Args...> {
  return member_call<RC, Class, Args...>(&object, member);
}

template <typename Signature>
struct DirectEvent;

template <typename... Args>
struct DirectEvent<void(Args...)> {
  std::vector<std::unique_ptr<ICall<void(Args...)>>> d_callbacks;

  DirectEvent(DirectEvent const&) = delete;
  void operator=(DirectEvent const&) = delete;

 public:
  DirectEvent() {}
  template <typename T>
  auto operator+=(T&& callback) -> DirectEvent& {
    this->d_callbacks.emplace_back(
        new Call<T, void(Args...)>(std::forward<T>(callback)));
    return *this;
  }
  template <typename T>
  auto operator-=(T&& callback) -> DirectEvent& {
    Call<T, void(Args...)> tmp(std::forward<T>(callback));
    auto it = remove_if(this->d_callbacks.begin(), this->d_callbacks.end(),
                        [&](std::unique_ptr<ICall<void(Args...)>>& other) {
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

}  // namespace ED

#endif
