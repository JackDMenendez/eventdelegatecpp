#ifndef EVENTDELEGATECPP_EVENT_HPP
#define EVENTDELEGATECPP_EVENT_HPP
#include <atomic>
#include <functional>
#include <iostream>
#include <memory>
#include <print>
#include <vector>
#include "senders/default_sender.h"

namespace EDCPP {
template <typename Signature>
struct ICall;

template <typename... Args>
struct ICall<void(Args...)> {
  virtual ~ICall() noexcept { std::cout << "~ICall()\n"; }
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
  virtual ~Call() noexcept { std::cout << "~Call()\n"; }
};

template <typename Signature>
struct ICallBackWrapper;

template <typename... Args>
struct ICallBackWrapper<void(Args...)> {
  virtual ~ICallBackWrapper() noexcept { std::cout << "~ICallBackWrapper()\n"; }
  virtual bool do_cmp(ICallBackWrapper* other) const noexcept = 0;
  virtual void do_call(Args... args) noexcept = 0;
  virtual uint64_t get_subscriber_id() const noexcept = 0;
  virtual void set_subscriber_id(uint64_t id) noexcept = 0;
};  // namespace EDCPP

template <typename T, typename Signature>
struct CallBackWrapper;
// need different one of these for closure style lambdas?
template <typename T, typename... Args>
struct CallBackWrapper<T, void(Args...)> : ICallBackWrapper<void(Args...)> {
  T d_callback;
  std::uint64_t subscriber_id;
  template <typename S>
  CallBackWrapper(S&& callback) : d_callback(std::forward<S>(callback)) {}
  CallBackWrapper(CallBackWrapper& call)
      : d_callback(std::forward<T>(call.d_callback)) {}
  CallBackWrapper& operator=(CallBackWrapper& call) {
    d_callback = std::forward<T>(call.d_callback);
  }
  // CallBackWrapper() : d_callback(nullptr) { std::println(std::stdout,
  // "CallBackWrapper()"); }
  bool do_cmp(ICallBackWrapper<void(Args...)>* other) const noexcept override {
    // CallBackWrapper<T, void(Args...)>* tmp = dynamic_cast<CallBackWrapper<T,
    // void(Args...)>*>(other);
    return this->get_subscriber_id() == other->get_subscriber_id();
  }
  void do_call(Args... args) noexcept override {
    return this->d_callback(std::forward<Args>(args)...);
  }
  std::uint64_t get_subscriber_id() const noexcept override {
    return subscriber_id;
  }
  void set_subscriber_id(std::uint64_t id) noexcept override {
    subscriber_id = id;
  }
  ~CallBackWrapper() noexcept override { std::cout << "~CallBackWrapper()\n"; }
};
template <typename RC, typename Class, typename... Args>
class member_call {
        union {
                Class* d_object;
                const Class* c_object;
                volatile Class* v_object;
        };
        union {
                RC(Class::* d_member)(Args...);
                RC(Class::* c_member)(Args...) const;
                RC(Class::* v_member)(Args...) volatile;
        };

        public:
        member_call(const Class* object, RC(Class::* member)(Args...) const) noexcept
                : c_object(object), c_member(member) {}
        member_call(const Class* object,
                    RC(Class::* member)(Args...) volatile) noexcept
                : v_object(object), v_member(member) {}
        member_call(Class* object, RC(Class::* member)(Args...)) noexcept
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
                std::cout << "~member_call()\n";
        }
};
template <typename RC, typename Class, typename... Args>
auto member(Class& object, RC(Class::* member)(Args...))
-> member_call<RC, Class, Args...> {
        return member_call<RC, Class, Args...>(&object, member);
}
template <typename RC, typename Class, typename... Args>
auto member(const Class& object, RC(Class::* member)(Args...) const)
-> member_call<RC, Class, Args...> {
        return member_call<RC, Class, Args...>(&object, member);
}

template <typename RC, typename Class, typename... Args>
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

 public:
  IntermediateMemberWrapper(const Class* object,
                            RC (Class::*GiftWrapper)(Args...) const) noexcept
      : c_object(object), c_member(GiftWrapper) {}
  IntermediateMemberWrapper(const Class* object,
                            RC (Class::*GiftWrapper)(Args...) volatile) noexcept
      : v_object(object), v_member(GiftWrapper) {}
  IntermediateMemberWrapper(Class* object,
                            RC (Class::*GiftWrapper)(Args...)) noexcept
      : d_object(object), d_member(GiftWrapper) {}
  auto operator()(Args... args) -> RC {
    return (this->d_object->*this->d_member)(std::forward<Args>(args)...);
  }
  auto operator()(Args... args) const -> RC {
    return (this->c_object->*this->c_member)(std::forward<Args>(args)...);
  }
  auto operator()(Args... args) volatile -> RC {
    return (this->v_object->*this->v_member)(std::forward<Args>(args)...);
  }
  auto operator==(IntermediateMemberWrapper const& other) const noexcept
      -> bool {
    return this->d_object == other.d_object && this->d_member == other.d_member;
  }
  auto operator!=(IntermediateMemberWrapper const& other) const noexcept
      -> bool {
    return !(*this == other);
  }
  ~IntermediateMemberWrapper() noexcept {
    std::cout << "~IntermediateMemberWrapper()\n";
  }
};
/*
template<typename RC, typename Anonymous,typename.. Args>
auto delegate(Class&
*/
template <typename RC, typename Class, typename... Args>
auto GiftWrapper(Class& object, RC (Class::*GiftWrapper)(Args...))
    -> IntermediateMemberWrapper<RC, Class, Args...> {
  return IntermediateMemberWrapper<RC, Class, Args...>(&object, GiftWrapper);
}
template <typename RC, typename Class, typename... Args>
auto GiftWrapper(const Class& object, RC (Class::*GiftWrapper)(Args...) const)
    -> IntermediateMemberWrapper<RC, Class, Args...> {
  return IntermediateMemberWrapper<RC, Class, Args...>(&object, GiftWrapper);
}

static std::atomic_uint64_t ID_Generator = 0;
static_assert(std::atomic_uint64_t::is_always_lock_free);

template <typename Signature>
class Delegate;
template <typename... Args>
class Delegate<void(Args...)> {
 private:
  std::shared_ptr<ICallBackWrapper<void(Args...)>> call_back;

 protected:
  std::uint64_t subscriber_id;

 public:
  std::uint64_t get_Subscriber_ID() const noexcept { return subscriber_id; }

  /**
   * @brief Handles functions excluding lambdas and GiftWrapper functions.
   */
  template <typename T>
    requires std::is_function<typename std::remove_reference<T>::type>::value
  Delegate(T&& callback)
      : call_back(std::make_shared<CallBackWrapper<T, void(Args...)>>(
            std::forward<T>(callback))) {
    subscriber_id = ID_Generator++;
  }
  /** Handles GiftWrapper functions include const and volatile */
  template <typename RC, typename Class>
    requires !std::is_invocable<
        typename std::remove_reference<Class>::type>::value
             Delegate(Class & object, RC (Class::*method)(Args...))
      : Delegate(GiftWrapper(object, method)) {
    subscriber_id = ID_Generator++;
  }
  /** Simple lambdas */
  template <typename T>
    requires(!std::is_function<
                typename std::remove_reference<T>::type>::value) &&
            std::is_invocable<typename std::remove_reference<T>::type,
                              Args...>::value
  Delegate(T&& callback)
      : call_back(std::make_shared<CallBackWrapper<T, void(Args...)>>(
            std::forward<T>(callback))) {
    subscriber_id = ID_Generator++;
  }
  void operator()(Args... args) { call_back->do_call(args...); }
  ~Delegate() noexcept { std::cout << "~Delegate()\n"; }
};
// https://stackoverflow.com/questions/23973914/c-like-delegates-in-c
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
    auto it =
        remove_if(this->d_callbacks.begin(), this->d_callbacks.end(),
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

// ----------------------------------------------------------------------------

}  // namespace EDCPP
#endif
