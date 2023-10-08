#ifndef DELEGATE_H
#define DELEGATE_H
#include <exception>
#include <type_traits>
#include <atomic>
#include <memory>
#include <utility>
#include <iostream>
#include "ed_base.h"
#include "event_concepts.h"
#include "ICallBackWrapper.h"
#include "CallBackWrapper.h"
#include "IntermediateMemberWrapper.h"
#include "MethodWrapper.h"
#include "IDelegateSharedWrapper.h"
#include "senders/default_sender.h"
namespace EDCPP {
static std::atomic_uint64_t ID_Generator = 0;
template <typename Signature>
class Delegate;
template <typename RC, typename... Args>
class Delegate<RC(Args...)> {
  static_assert(!std::is_same<RC, void>::value,
                "Delegate function must return void");
};
template <typename... Args>
class Delegate<void(Args...)> : IDelegateSharedWrapper<Args...> {
 public:
  using FN = void(Args...);

 private:
  std::shared_ptr<ICallBackWrapper<FN>> call_back;

 protected:
 public:
  auto operator==(Delegate<FN> const& other) const noexcept -> bool {
    return call_back->do_cmp(other.call_back->get());
  }
  auto operator!=(Delegate<FN> const& other) const noexcept -> bool {
    return !(*this == other);
  }
  std::shared_ptr<ICallBackWrapper<void(Args...)>>& get_ptr() noexcept
      override {
    return call_back;
  }
  std::uint64_t get_Subscriber_ID() const noexcept {
    return call_back->get_subscriber_id();
  }
  bool get_except_flag() const noexcept {
    return call_back->throws_exception();
  }
  /**
   * @brief Handles functions excluding lambdas and MethodWrapper functions.
   */
  template <typename T>
    requires BasicFunction<T> && NotMemberFunction<T>
  Delegate(T&& callback)
      : call_back(std::make_shared<CallBackWrapper<T, FN>>(
            std::forward<T>(callback))) {
    call_back->set_subscriber_id(ID_Generator++);
    call_back->set_throws_exception(
        !std::is_nothrow_invocable<T, Args...>::value);
  }
  /**
   * @brief Handles MethodWrapper functions for member functions including const
   * and volatile
   *
   * @note For now, methods will all be treated as in the throws exception
   * regime since decltype does not preserve information about the exception
   * regime.
   */
  template <typename RC, typename Class>
    requires InstantiatedMethod<Class>
  Delegate(Class& object, RC (Class::*method)(Args...))
      : Delegate(MethodWrapper(object, method)) {
    call_back->set_subscriber_id(ID_Generator++);
    // The following always call set_trhow_exception(true);
    call_back->set_throws_exception(
        !std::is_nothrow_invocable_r_v<RC (Class::*)(Args...), decltype(method),
                                       Args...>);
  }
  /** lambdas */
  template <typename T>
    requires LambdaFunction<T, Args...> && NotMemberFunction<T>
  Delegate(T&& callback)
      : call_back(std::make_shared<CallBackWrapper<T, FN>>(
            std::forward<T>(callback))) {
    call_back->set_subscriber_id(ID_Generator++);
    call_back->set_throws_exception(
        !std::is_nothrow_invocable<T, Args...>::value);
  }
  void operator()(Args... args) {
    if (call_back->throws_exception()) {
      try {
        call_back->do_call_except(args...);
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      } catch (...) {
        std::cerr << "Unknown exception type caught\n";
      }
      return;
    }
    call_back->do_call_noexcept(args...);
  }
  ~Delegate() noexcept { std::cout << "~Delegate()\n"; }
};
}
#endif
