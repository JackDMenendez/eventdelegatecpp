#ifndef EVENTDELEGATECPP_EVENT_HPP
#define EVENTDELEGATECPP_EVENT_HPP
#include <functional>
#include <memory>
#include <vector>
#include "senders/default_sender.h"

namespace EDCPP {

template <typename Signature>
struct ICall;

template <typename... Args>
struct ICall<void(Args...)> {
  virtual ~ICall() noexcept = default;
  virtual auto do_cmp(ICall* other) noexcept -> bool = 0;
  virtual void do_call(Args... args) noexcept = 0;
};

template <typename T, typename Signature>
struct Call;
// need different one of these for closure style lambdas?
template <typename T, typename... Args>
struct Call<T, void(Args...)> : ICall<void(Args...)> {
  T d_callback;
  template <typename S>
  Call(S&& callback) : d_callback(std::forward<S>(callback)) {}

  auto do_cmp(ICall<void(Args...)>* other) noexcept -> bool {
    Call<T, void(Args...)>* tmp = dynamic_cast<Call<T, void(Args...)>*>(other);
    return tmp && this->d_callback == tmp->d_callback;
  }
  void do_call(Args... args) noexcept {
    return this->d_callback(std::forward<Args>(args)...);
  }
};

// https://stackoverflow.com/questions/23973914/c-like-delegates-in-c
template <typename Signature>
struct EventSubscription;

template <typename... Args>
struct EventSubscription<void(Args...)> {
  std::vector<std::unique_ptr<ICall<void(Args...)>>> d_callbacks;

  EventSubscription(EventSubscription const&) = delete;
  void operator=(EventSubscription const&) = delete;

 public:
  EventSubscription() {}
  template <typename T>
  auto operator+=(T&& callback) -> EventSubscription& {
    this->d_callbacks.emplace_back(
        new Call<T, void(Args...)>(std::forward<T>(callback)));
    return *this;
  }
  template <typename T>
  auto operator-=(T&& callback) -> EventSubscription& {
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

// ----------------------------------------------------------------------------

template <typename RC, typename Class, typename... Args>
class member_call {
  Class* d_object;
  RC (Class::*d_member)(Args...);

 public:
  member_call(Class* object, RC (Class::*member)(Args...))
      : d_object(object), d_member(member) {}
  auto operator()(Args... args) -> RC {
    return (this->d_object->*this->d_member)(std::forward<Args>(args)...);
  }
  auto operator==(member_call const& other) const -> bool {
    return this->d_object == other.d_object && this->d_member == other.d_member;
  }
  auto operator!=(member_call const& other) const -> bool {
    return !(*this == other);
  }
};
/*
template<typename Anon>
class lambda_call : public function<Anon> {
public:
                lambda_call(Anon object) : d_object(object) {
                }
                auto operator== (lambda_call const& other) const -> bool {
                                return this->d_object == other.d_object;
                }
                auto operator!= (lambda_call const& other) const -> bool {
                                return !(*this == other);
                }
};*/
/*
template<typename RC, typename Anonymous,typename.. Args>
auto delegate(Class&
*/
template <typename RC, typename Class, typename... Args>
auto member(Class& object, RC (Class::*member)(Args...))
    -> member_call<RC, Class, Args...> {
  return member_call<RC, Class, Args...>(&object, member);
}
}  // namespace EDCPP
#endif
