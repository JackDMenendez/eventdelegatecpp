#ifndef DELEGATE_H
#define DELEGATE_H
#include <atomic>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include "CallBackWrapper.h"
#include "ICallBackWrapper.h"
#include "IDelegateSharedWrapper.h"
#include "IntermediateMemberWrapper.h"
#include "MethodWrapper.h"
#include "ed_base.h"
#include "event_concepts.h"
#include "senders/default_sender.h"
EDCPP_BEGIN
static std::atomic_uint64_t ID_Generator = 0;
constexpr auto EmptyDelegateException = "attempt to use empty delegate";
/**
 * @brief An exception thrown when the delegates internal Delegate is empty.
 *
 * @details Can happen if the delegate is default constructed or the delegate is
 * assigned to another delegate that is empty.
 */
EDCPP_EXPORT class EmptyDelegate : public std::exception {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return EmptyDelegateException;
  }
};

extern "C++" [[noreturn]] void EDCPP_CALL throw_empty_delegate();

struct _Unforced {  // tag to distinguish bind() from bind<R>()
  explicit _Unforced() = default;
};

// helper to give INVOKE an explicit return type; avoids undesirable Expression
// SFINAE
template <class _Rx>
struct _Invoker_ret {  // selected for all _Rx other than _Unforced
  template <class _Fx,
            class... _Valtys,
            _STD enable_if_t<_STD _Select_invoke_traits<_Fx, _Valtys...>::
                                 template _Is_invocable_r<_Rx>::value,
                             int> = 0>
  static constexpr _Rx _Call(_Fx&& _Func, _Valtys&&... _Vals) noexcept(
      _STD _Select_invoke_traits<_Fx,
                                 _Valtys...>::
          template _Is_nothrow_invocable_r<_Rx>::value) {  // INVOKE, implicitly
                                                           // converted
    if constexpr (_STD is_void_v<_Rx>) {
      static_assert(
          _STD _Select_invoke_traits<
              _Fx, _Valtys...>::_Is_nothrow_invocable::value ==
          _STD _Select_invoke_traits<
              _Fx, _Valtys...>::template _Is_nothrow_invocable_r<_Rx>::value);
      _STD invoke(static_cast<_Fx&&>(_Func), static_cast<_Valtys&&>(_Vals)...);
    } else {
      return _STD invoke(static_cast<_Fx&&>(_Func),
                         static_cast<_Valtys&&>(_Vals)...);
    }
  }
};

template <>
struct _Invoker_ret<_Unforced> {  // selected for _Rx being _Unforced
  template <class _Fx, class... _Valtys>
  static constexpr auto _Call(_Fx&& _Func, _Valtys&&... _Vals) noexcept(
      _STD _Select_invoke_traits<_Fx, _Valtys...>::_Is_nothrow_invocable::value)
      -> decltype(_STD invoke(
          static_cast<_Fx&&>(_Func),
          static_cast<_Valtys&&>(_Vals)...)) {  // INVOKE, unchanged
    return _STD invoke(static_cast<_Fx&&>(_Func),
                       static_cast<_Valtys&&>(_Vals)...);
  }
};

EDCPP_EXPORT template <class _Fty>
class Delegate;

template <class _Ty>
inline constexpr bool _Testable_callable_v =
    _STD disjunction_v<_STD is_pointer<_Ty>,
                       _STD _Is_specialization<_Ty, Delegate>,
                       _STD is_member_pointer<_Ty>>;

template <class _Ty>
bool _Test_callable(
    const _Ty&
        _Arg) noexcept {  // determine whether std::Delegate must store _Arg
  if constexpr (_Testable_callable_v<_Ty>) {
    return !!_Arg;
  } else {
    return true;
  }
}

template <class _Rx, class... _Types>
class __declspec(novtable)
    _Delegate_function_base {  // abstract base for implementation types
 public:
  // virtual _Delegate_function_base* _Copy(void*) const = 0;
  // virtual _Delegate_function_base* _Move(void*) noexcept = 0;
  virtual _Rx _Do_call(_Types&&...) = 0;
  virtual const type_info& _Target_type() const noexcept = 0;
  // virtual void _Delete_this(bool) noexcept = 0;

  _Delegate_function_base() = default;
  _Delegate_function_base(const _Delegate_function_base&) = delete;
  _Delegate_function_base& operator=(const _Delegate_function_base&) = delete;
  virtual ~_Delegate_function_base() noexcept = default;

 private:
  virtual const void* _Get() const noexcept = 0;
};

inline constexpr size_t _Space_size =
    (_STD _Small_object_num_ptrs - 1) * sizeof(void*);

template <class _Impl>  // determine whether _Impl must be dynamically allocated
inline constexpr bool _Is_large = sizeof(_Impl) > _Space_size ||
                                  alignof(_Impl) > alignof(max_align_t) ||
                                  !_Impl::_Nothrow_move::value;

template <class _Callable, class _Rx, class... _Types>
class _Delegate_function_impl_no_alloc final
    : public _Delegate_function_base<_Rx, _Types...> {
  // derived class for specific implementation types that don't use allocators
 public:
  using _Mybase = _Delegate_function_base<_Rx, _Types...>;
  using _Nothrow_move = _STD is_nothrow_move_constructible<_Callable>;

  template <class _Other,
            _STD enable_if_t<!_STD is_same_v<_Delegate_function_impl_no_alloc,
                                             _STD decay_t<_Other>>,
                             int> = 0>
  explicit _Delegate_function_impl_no_alloc(_Other&& _Val)
      : _Callee(_STD forward<_Other>(_Val)) {}

  // dtor non-virtual due to _Delete_this()

 private:
  //_Mybase* _Copy(void* _Where) const override {
  //  if constexpr (_STD _Is_large<_Delegate_function_impl_no_alloc>) {
  //    return _STD _Global_new<_Delegate_function_impl_no_alloc>(_Callee);
  //  } else {
  //    return ::new (_Where) _Delegate_function_impl_no_alloc(_Callee);
  //  }
  //}

  //_Mybase* _Move(void* _Where) noexcept override {
  //  if constexpr (_STD _Is_large<_Delegate_function_impl_no_alloc>) {
  //    return nullptr;
  //  } else {
  //    return ::new (_Where)
  //        _Delegate_function_impl_no_alloc(_STD move(_Callee));
  //  }
  //}

  _Rx _Do_call(_Types&&... _Args) override {  // call wrapped Delegate
    if constexpr (_STD is_void_v<_Rx>) {
      (void)_STD invoke(_Callee, _STD forward<_Types>(_Args)...);
    } else {
      return _STD invoke(_Callee, _STD forward<_Types>(_Args)...);
    }
  }

  const type_info& _Target_type() const noexcept override { _CSTD abort(); }

  const void* _Get() const noexcept override { return _STD addressof(_Callee); }

  //  void _Delete_this(bool _Dealloc) noexcept override {  // destroy self
  //    this->~_Delegate_function_impl_no_alloc();
  //    if (_Dealloc) {
  //      _Deallocate<alignof(_Delegate_function_impl_no_alloc)>(
  //          this, sizeof(_Delegate_function_impl_no_alloc));
  //    }
  //  }
  _STD function<_Rx(_Types...)> _Callee;  // the wrapped Delegate object
  //_Callable _Callee;
};

template <class _Ret, class... _Types>
class _Delegate_function_class : public _STD _Arg_types<_Types...> {
 public:
  using result_type = _Ret;

  using _Ptrt = _Delegate_function_base<_Ret, _Types...>;

  _Delegate_function_class() noexcept { _Set(nullptr); }

  _Ret operator()(_Types... _Args) const {
    if (_Empty()) {
      throw_empty_delegate();
    }
    const auto _Impl = _Getimpl();
    return _Impl->_Do_call(_STD forward<_Types>(_Args)...);
  }

  ~_Delegate_function_class() noexcept {
    //_Tidy();
  }

 protected:
  template <class _Fx, class _Delegate>
  using _Enable_if_callable_t = _STD enable_if_t<
      _STD conjunction_v<
          _STD negation<_STD is_same<_STD _Remove_cvref_t<_Fx>, _Delegate>>,
          _STD _Is_invocable_r<_Ret, _STD decay_t<_Fx>&, _Types...>>,
      int>;

  bool _Empty() const noexcept { return !_Getimpl(); }

  // void _Reset_copy(
  //     const _Delegate_function_class& _Right) {  // copy _Right's stored
  //     object
  //   if (!_Right._Empty()) {
  //     _Set(_Right._Getimpl()->_Copy(&_Mystorage));
  //   }
  // }

  // void _Reset_move(_Delegate_function_class&&
  //                      _Right) noexcept {  // move _Right's stored object
  //   if (!_Right._Empty()) {
  //     if (_Right._Local()) {  // move and tidy
  //       _Set(_Right._Getimpl()->_Move(&_Mystorage));
  //       _Right._Tidy();
  //     } else {  // steal from _Right
  //       _Set(_Right._Getimpl());
  //       _Right._Set(nullptr);
  //     }
  //   }
  // }

  template <class _Fx>
  void _Reset(_Fx&& _Val) {  // store copy of _Val
    if (!_Test_callable(
            _Val)) {  // null member pointer/Delegate pointer/std::Delegate
      return;         // already empty
    }

    using _Impl =
        _Delegate_function_impl_no_alloc<_STD decay_t<_Fx>, _Ret, _Types...>;
    if constexpr (_Is_large<_Impl>) {
      // dynamically allocate _Val
      _Set(_Global_new<_Impl>(_STD forward<_Fx>(_Val)));
    } else {
      // store _Val in-situ
      _Set(::new (static_cast<void*>(&_Mystorage))
               _Impl(_STD forward<_Fx>(_Val)));
    }
  }

  // void _Tidy() noexcept {
  //   if (!_Empty()) {  // destroy callable object and maybe delete it
  //     //_Getimpl()->_Delete_this(!_Local());
  //     _Set(nullptr);
  //   }
  // }

  void _Swap(_Delegate_function_class&
                 _Right) noexcept {  // swap contents with contents of _Right
    if (!_Local() && !_Right._Local()) {  // just swap pointers
      _Ptrt* _Temp = _Getimpl();
      _Set(_Right._Getimpl());
      _Right._Set(_Temp);
    } else {  // do three-way move
      _Delegate_function_class _Temp;
      _Temp._Reset_move(_STD move(*this));
      _Reset_move(_STD move(_Right));
      _Right._Reset_move(_STD move(_Temp));
    }
  }

 private:
  bool _Local() const noexcept {  // test for locally stored copy of object
    return _Getimpl() == static_cast<const void*>(&_Mystorage);
  }

  union _Storage {        // storage for small objects (basic_string is small)
    max_align_t _Dummy1;  // for maximum alignment
    char _Dummy2[_Space_size];                  // to permit aliasing
    _Ptrt* _Ptrs[_STD _Small_object_num_ptrs];  // _Ptrs[_Small_object_num_ptrs
                                                // - 1] is reserved
  };

  _Storage _Mystorage;
  enum {
    _EEN_IMPL = _STD _Small_object_num_ptrs - 1
  };                                  // helper for expression evaluator
  _Ptrt* _Getimpl() const noexcept {  // get pointer to object
    return _Mystorage._Ptrs[_STD _Small_object_num_ptrs - 1];
  }

  void _Set(_Ptrt* _Ptr) noexcept {  // store pointer to object
    _Mystorage._Ptrs[_STD _Small_object_num_ptrs - 1] = _Ptr;
  }
};

template <class _Tx>
struct _Get_Delegate_impl {
  static_assert(
      _STD _Always_false<_Tx>,
      "std::Delegate only accepts Delegate types as template arguments.");
};

#define _GET_DELEGATE_IMPL(CALL_OPT, X1, X2, X3)                               \
  template <class _Ret, class... _Types>                                       \
  struct _Get_Delegate_impl<_Ret CALL_OPT(_Types...)> { /* determine type from \
                                                           argument list */    \
    using type = _Delegate_function_class<_Ret, _Types...>;                    \
  };

_NON_MEMBER_CALL(_GET_DELEGATE_IMPL, X1, X2, X3)
#undef _GET_DELEGATE_IMPL

EDCPP_EXPORT template <class _Fty>
class Delegate
    : public _Get_Delegate_impl<_Fty>::type {  // wrapper for callable objects
 private:
  using _Mybase = typename _Get_Delegate_impl<_Fty>::type;

 public:
  Delegate() noexcept {}

  Delegate(nullptr_t) noexcept {}

  // Delegate(const Delegate& _Right) { this->_Reset_copy(_Right); }

  template <class _Fx,
            typename _Mybase::template _Enable_if_callable_t<_Fx, Delegate> = 0>
  Delegate(_Fx&& _Func) {
    this->_Reset(_STD forward<_Fx>(_Func));
  }

  // Delegate& operator=(const Delegate& _Right) {
  //   Delegate(_Right).swap(*this);
  //   return *this;
  // }

  // Delegate(Delegate&& _Right) noexcept { this->_Reset_move(_STD
  // move(_Right)); }

  // Delegate& operator=(Delegate&& _Right) noexcept /* strengthened */ {
  //   if (this != _STD addressof(_Right)) {
  //     this->_Tidy();
  //     this->_Reset_move(_STD move(_Right));
  //   }
  //   return *this;
  // }

  template <class _Fx,
            typename _Mybase::template _Enable_if_callable_t<_Fx, Delegate> = 0>
  Delegate& operator=(_Fx&& _Func) {
    Delegate(_STD forward<_Fx>(_Func)).swap(*this);
    return *this;
  }

  // Delegate& operator=(nullptr_t) noexcept {
  //   this->_Tidy();
  //   return *this;
  // }

  template <class _Fx>
  Delegate& operator=(_STD reference_wrapper<_Fx> _Func) noexcept {
    // this->_Tidy();
    this->_Reset(_Func);
    return *this;
  }

  // void swap(Delegate& _Right) noexcept { this->_Swap(_Right); }

  explicit operator bool() const noexcept { return !this->_Empty(); }
};

#if _HAS_CXX17
#define _DELEGATE_POINTER_DEDUCTION_GUIDE(CALL_OPT, X1, X2, X3) \
  template <class _Ret, class... _Types>                        \
  Delegate(_Ret(CALL_OPT*)(_Types...))                          \
      -> Delegate<_Ret(_Types...)>;  // intentionally discards CALL_OPT

_NON_MEMBER_CALL(_DELEGATE_POINTER_DEDUCTION_GUIDE, X1, X2, X3)
#undef _DELEGATE_POINTER_DEDUCTION_GUIDE

template <class _Fx, class = void>
struct _Deduce_signature {};  // can't deduce signature when &_Fx::operator() is
                              // missing, inaccessible, or ambiguous

template <class _Fx>
struct _Deduce_signature<_Fx, _STD void_t<decltype(&_Fx::operator())>>
    : _STD _Is_memfunptr<decltype(&_Fx::operator())>::_Guide_type {
};  // N4950 [func.wrap.func.con]/16.1

template <class _Fx>
Delegate(_Fx) -> Delegate<typename _Deduce_signature<_Fx>::type>;
#endif  // _HAS_CXX17

EDCPP_EXPORT template <class _Fty>
void swap(Delegate<_Fty>& _Left, Delegate<_Fty>& _Right) noexcept {
  _Left.swap(_Right);
}

EDCPP_EXPORT template <class _Fty>
[[nodiscard]] bool operator==(const Delegate<_Fty>& _Other,
                              nullptr_t) noexcept {
  return !_Other;
}

#if !_HAS_CXX20
template <class _Fty>
[[nodiscard]] bool operator==(nullptr_t,
                              const Delegate<_Fty>& _Other) noexcept {
  return !_Other;
}

template <class _Fty>
[[nodiscard]] bool operator!=(const Delegate<_Fty>& _Other,
                              nullptr_t) noexcept {
  return static_cast<bool>(_Other);
}

template <class _Fty>
[[nodiscard]] bool operator!=(nullptr_t,
                              const Delegate<_Fty>& _Other) noexcept {
  return static_cast<bool>(_Other);
}
#endif  // !_HAS_CXX20

#if _HAS_CXX23
// _Move_only_Delegate_data is defined as an array of pointers.
// The first element is always a pointer to _Move_only_Delegate_base::_Impl_t;
// it emulates a vtable pointer. The other pointers are used as storage for a
// small functor; if the functor does not fit in, the second pointer is the
// pointer to allocated storage, the rest are unused.
union alignas(max_align_t) _Move_only_Delegate_data {
  void* _Pointers[_STD _Small_object_num_ptrs];
  const void* _Impl;
  char _Data;  // For aliasing

  template <class _Fn>
  static constexpr size_t _Buf_offset =
      alignof(_Fn) <= sizeof(_Impl)
          ? sizeof(_Impl)  // Store _Fn immediately after _Impl
          : alignof(_Fn);  // Pad _Fn to next alignment

  template <class _Fn>
  static constexpr size_t _Buf_size = sizeof(_Pointers) - _Buf_offset<_Fn>;

  template <class _Fn>
  [[nodiscard]] void* _Buf_ptr() noexcept {
    return &_Data + _Buf_offset<_Fn>;
  }

  template <class _Fn>
  [[nodiscard]] _Fn* _Small_fn_ptr() const noexcept {
    // cast away const to avoid complication of const propagation to here;
    // const correctness is still enforced by _Move_only_Delegate_call
    // specializations.
    return static_cast<_Fn*>(
        const_cast<_Move_only_Delegate_data*>(this)->_Buf_ptr<_Fn>());
  }

  template <class _Fn>
  [[nodiscard]] _Fn* _Large_fn_ptr() const noexcept {
    return static_cast<_Fn*>(_Pointers[1]);
  }

  void _Set_large_fn_ptr(void* const _Value) noexcept { _Pointers[1] = _Value; }
};

// Size of a large Delegate. Treat an empty Delegate as if it has this size.
// Treat a small Delegate as if it has this size too if it fits and is trivially
// copyable.
inline constexpr size_t _Minimum_Delegate_size = 2 * sizeof(void*);

// The below Delegates are __stdcall as they are called by pointers from
// _Move_only_Delegate_base::_Impl_t. (We use explicit __stdcall to make the ABI
// stable for translation units with different calling convention options.)
// Non-template Delegates are still defined inline, as the compiler may be able
// to devirtualize some calls.

template <class _Rx, class... _Types>
[[noreturn]] _Rx __stdcall _Delegate_not_callable(
    const _Move_only_Delegate_data&,
    _Types&&...) noexcept {
  _CSTD abort();  // Unlike std::Delegate, move_only_Delegate doesn't throw
                  // bad_Delegate_call (N4950 [func.wrap.move.inv]/2)
}

template <class _Vt, class _VtInvQuals, class _Rx, bool _Noex, class... _Types>
[[nodiscard]] _Rx __stdcall _Delegate_inv_small(
    const _Move_only_Delegate_data& _Self,
    _Types&&... _Args) noexcept(_Noex) {
  if constexpr (_STD is_void_v<_Rx>) {
    (void)_STD invoke(static_cast<_VtInvQuals>(*_Self._Small_fn_ptr<_Vt>()),
                      _STD forward<_Types>(_Args)...);
  } else {
    return _STD invoke(static_cast<_VtInvQuals>(*_Self._Small_fn_ptr<_Vt>()),
                       _STD forward<_Types>(_Args)...);
  }
}

template <class _Vt, class _VtInvQuals, class _Rx, bool _Noex, class... _Types>
[[nodiscard]] _Rx __stdcall _Delegate_inv_large(
    const _Move_only_Delegate_data& _Self,
    _Types&&... _Args) noexcept(_Noex) {
  if constexpr (_STD is_void_v<_Rx>) {
    (void)_STD invoke(static_cast<_VtInvQuals>(*_Self._Large_fn_ptr<_Vt>()),
                      _STD forward<_Types>(_Args)...);
  } else {
    return _STD invoke(static_cast<_VtInvQuals>(*_Self._Large_fn_ptr<_Vt>()),
                       _STD forward<_Types>(_Args)...);
  }
}

template <class _Vt>
void __stdcall _Delegate_move_small(_Move_only_Delegate_data& _Self,
                                    _Move_only_Delegate_data& _Src) noexcept {
  const auto _Src_fn_ptr = _Src._Small_fn_ptr<_Vt>();
  ::new (_Self._Buf_ptr<_Vt>()) _Vt(_STD move(*_Src_fn_ptr));
  _Src_fn_ptr->~_Vt();
  _Self._Impl = _Src._Impl;
}

template <size_t _Size>
void __stdcall _Delegate_move_memcpy(_Move_only_Delegate_data& _Self,
                                     _Move_only_Delegate_data& _Src) noexcept {
  _CSTD memcpy(&_Self._Data, &_Src._Data,
               _Size);  // Copy Impl* and functor data
}

inline void __stdcall _Delegate_move_large(
    _Move_only_Delegate_data& _Self,
    _Move_only_Delegate_data& _Src) noexcept {
  _CSTD memcpy(&_Self._Data, &_Src._Data,
               _Minimum_Delegate_size);  // Copy Impl* and functor data
}

template <class _Vt>
void __stdcall _Delegate_destroy_small(
    _Move_only_Delegate_data& _Self) noexcept {
  _Self._Small_fn_ptr<_Vt>()->~_Vt();
}

inline void __stdcall _Delegate_deallocate_large_default_aligned(
    _Move_only_Delegate_data& _Self) noexcept {
  ::operator delete(_Self._Large_fn_ptr<void>());
}

template <size_t _Align>
void __stdcall _Delegate_deallocate_large_overaligned(
    _Move_only_Delegate_data& _Self) noexcept {
  _STL_INTERNAL_STATIC_ASSERT(_Align > __STDCPP_DEFAULT_NEW_ALIGNMENT__);
#ifdef __cpp_aligned_new
  ::operator delete(_Self._Large_fn_ptr<void>(), _STD align_val_t{_Align});
#else   // ^^^ __cpp_aligned_new / !__cpp_aligned_new vvv
  ::operator delete(_Self._Large_fn_ptr<void>());
#endif  // __cpp_aligned_new
}

template <class _Vt>
void __stdcall _Delegate_destroy_large(
    _Move_only_Delegate_data& _Self) noexcept {
  const auto _Pfn = _Self._Large_fn_ptr<_Vt>();
  _Pfn->~_Vt();
#ifdef __cpp_aligned_new
  if constexpr (alignof(_Vt) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
    ::operator delete(static_cast<void*>(_Pfn), _STD align_val_t{alignof(_Vt)});
  } else
#endif  // __cpp_aligned_new
  {
    ::operator delete(static_cast<void*>(_Pfn));
  }
}

template <class _Vt>
inline constexpr size_t
    _Delegate_small_copy_size =  // We copy Impl* and the functor data at once
    _Move_only_Delegate_data::_Buf_offset<_Vt>  // Impl* plus possible alignment
    + (size_t{sizeof(_Vt) + sizeof(void*) - 1} &
       ~size_t{sizeof(void*) - 1});  // size in whole pointers

template <class _Vt, class... _CTypes>
[[nodiscard]] void* _Delegate_new_large(_CTypes&&... _Args) {
  struct [[nodiscard]] _Guard_type {
    void* _Ptr;

    ~_Guard_type() {
      // _Ptr is not nullptr only if an exception is thrown as a result of _Vt
      // construction. Check _Ptr before calling operator delete to save the
      // call in the common case.
      if (_Ptr) {
#ifdef __cpp_aligned_new
        if constexpr (alignof(_Vt) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
          ::operator delete(_Ptr, _STD align_val_t{alignof(_Vt)});
        } else
#endif  // __cpp_aligned_new
        {
          ::operator delete(_Ptr);
        }
      }
    }
  };

  void* _Ptr;
#ifdef __cpp_aligned_new
  if constexpr (alignof(_Vt) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
    _Ptr = ::operator new(sizeof(_Vt), _STD align_val_t{alignof(_Vt)});
  } else
#endif  // __cpp_aligned_new
  {
    _Ptr = ::operator new(sizeof(_Vt));
  }
  _Guard_type _Guard{_Ptr};
  ::new (_Ptr) _Vt(_STD forward<_CTypes>(_Args)...);
  _Guard._Ptr = nullptr;
  return _Ptr;
}

template <class _Rx, bool _Noexcept, class... _Types>
class _Move_only_Delegate_base {
 public:
  // TRANSITION, DevCom-1208330: use noexcept(_Noexcept) instead
  template <bool>
  struct _Invoke_t {
    using _Call = _Rx(__stdcall*)(const _Move_only_Delegate_data&, _Types&&...);
  };

  template <>
  struct _Invoke_t<true> {
    using _Call = _Rx(__stdcall*)(const _Move_only_Delegate_data&,
                                  _Types&&...) _NOEXCEPT_FNPTR;
  };

  struct _Impl_t {  // A per-callable-type structure acting as a virtual
                    // Delegate table.
    // Using vtable emulations gives more flexibility for optimizations and
    // reduces the amount of RTTI data. (The RTTI savings may be significant as
    // with lambdas and binds there may be many distinct callable types. Here we
    // don't have a distinct wrapper class for each callable type, only distinct
    // Delegates when needed.)

    // _Move and _Destroy are nullptr if trivial. Besides being an optimization,
    // this enables assigning an empty Delegate from a DLL that is unloaded
    // later, and then safely moving/destroying that empty Delegate.

    // Calls target
    typename _Invoke_t<_Noexcept>::_Call _Invoke;
    // Moves the data, including pointer to "vtable", AND destroys old data (not
    // resetting its "vtable"). nullptr if we can trivially move two pointers.
    void(__stdcall* _Move)(_Move_only_Delegate_data&,
                           _Move_only_Delegate_data&) _NOEXCEPT_FNPTR;
    // Destroys data (not resetting its "vtable").
    // nullptr if destruction is a no-op.
    void(__stdcall* _Destroy)(_Move_only_Delegate_data&) _NOEXCEPT_FNPTR;
  };

  _Move_only_Delegate_data _Data;

  _Move_only_Delegate_base() noexcept = default;  // leaves fields uninitialized

  _Move_only_Delegate_base(_Move_only_Delegate_base&& _Other) noexcept {
    _Checked_move(_Data, _Other._Data);
    _Other._Reset_to_null();
  }

  void _Construct_with_null() noexcept {
    _Data._Impl = nullptr;
    _Data._Set_large_fn_ptr(nullptr);  // initialize, since we'll be copying it
  }

  void _Reset_to_null() noexcept { _Data._Impl = nullptr; }

  template <class _Vt, class _VtInvQuals, class... _CTypes>
  void _Construct_with_fn(_CTypes&&... _Args) {
    _Data._Impl = _Create_impl_ptr<_Vt, _VtInvQuals>();
    if constexpr (_Large_Delegate_engaged<_Vt>) {
      _Data._Set_large_fn_ptr(
          _Delegate_new_large<_Vt>(_STD forward<_CTypes>(_Args)...));
    } else {
      ::new (_Data._Buf_ptr<_Vt>()) _Vt(_STD forward<_CTypes>(_Args)...);
    }
  }

  static void _Checked_destroy(_Move_only_Delegate_data& _Data) noexcept {
    const auto _Impl = _Get_impl(_Data);
    if (_Impl->_Destroy) {
      _Impl->_Destroy(_Data);
    }
  }

  static void _Checked_move(_Move_only_Delegate_data& _Data,
                            _Move_only_Delegate_data& _Src) noexcept {
    const auto _Impl = _Get_impl(_Src);
    if (_Impl->_Move) {
      _Impl->_Move(_Data, _Src);
    } else {
      _Delegate_move_large(_Data, _Src);
    }
  }

  void _Move_assign(_Move_only_Delegate_base&& _Other) noexcept {
    // As specified in N4950 [func.wrap.move.ctor]/22, we are expected to first
    // move the new target, then finally destroy the old target. It is more
    // efficient to do the reverse - this way no temporary storage for the old
    // target will be used. In some cases when some operations are trivial, it
    // can be optimized, as the order change is unobservable, and everything is
    // noexcept here.
    const auto _This_impl = _Get_impl(_Data);
    const auto _Other_impl_move = _Get_impl(_Other._Data)->_Move;
    const auto _This_impl_destroy = _This_impl->_Destroy;

    if (!_Other_impl_move) {
      // Move is trivial, destroy first if needed
      if (_This_impl_destroy) {
        _This_impl_destroy(_Data);
      }
      _Delegate_move_large(_Data, _Other._Data);
    } else if (!_This_impl_destroy) {
      // Destroy is trivial, just move
      _Other_impl_move(_Data, _Other._Data);
    } else {
      // General case involving a temporary
      _Move_only_Delegate_data _Tmp;

      if (_This_impl->_Move) {
        _This_impl->_Move(_Tmp, _Data);
      } else {
        _Delegate_move_large(_Tmp, _Data);
      }

      _Other_impl_move(_Data, _Other._Data);
      _This_impl_destroy(_Tmp);
    }
    _Other._Reset_to_null();
  }

  void _Swap(_Move_only_Delegate_base& _Other) noexcept {
    _Move_only_Delegate_data _Tmp;

    _Checked_move(_Tmp, _Data);
    _Checked_move(_Data, _Other._Data);
    _Checked_move(_Other._Data, _Tmp);
  }

  [[nodiscard]] bool _Is_null() const noexcept {
    return _Data._Impl == nullptr;
  }

  template <class _Vt>
  static constexpr bool _Large_Delegate_engaged =
      alignof(_Vt) > alignof(max_align_t) ||
      sizeof(_Vt) > _Move_only_Delegate_data::_Buf_size<_Vt> ||
      !_STD is_nothrow_move_constructible_v<_Vt>;

  [[nodiscard]] auto _Get_invoke() const noexcept {
    return _Get_impl(_Data)->_Invoke;
  }

  [[nodiscard]] static const _Impl_t* _Get_impl(
      const _Move_only_Delegate_data& _Data) noexcept {
    static constexpr _Impl_t _Null_move_only_Delegate = {
        _Delegate_not_callable<_Rx, _Types...>,
        nullptr,
        nullptr,
    };

    const auto _Ret = static_cast<const _Impl_t*>(_Data._Impl);
    return _Ret ? _Ret : &_Null_move_only_Delegate;
  }

  template <class _Vt, class _VtInvQuals>
  [[nodiscard]] static constexpr _Impl_t _Create_impl() noexcept {
    _Impl_t _Impl{};
    if constexpr (_Large_Delegate_engaged<_Vt>) {
      _Impl._Invoke =
          _Delegate_inv_large<_Vt, _VtInvQuals, _Rx, _Noexcept, _Types...>;
      _Impl._Move = nullptr;

      if constexpr (_STD is_trivially_destructible_v<_Vt>) {
        if constexpr (alignof(_Vt) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
          _Impl._Destroy = _Delegate_deallocate_large_default_aligned;
        } else {
          _Impl._Destroy = _Delegate_deallocate_large_overaligned<alignof(_Vt)>;
        }
      } else {
        _Impl._Destroy = _Delegate_destroy_large<_Vt>;
      }
    } else {
      _Impl._Invoke =
          _Delegate_inv_small<_Vt, _VtInvQuals, _Rx, _Noexcept, _Types...>;

      if constexpr (_STD is_trivially_copyable_v<_Vt> &&
                    _STD is_trivially_destructible_v<_Vt>) {
        if constexpr ((_Delegate_small_copy_size<_Vt>) >
                      _Minimum_Delegate_size) {
          _Impl._Move = _Delegate_move_memcpy<_Delegate_small_copy_size<_Vt>>;
        } else {
          _Impl._Move = nullptr;
        }
      } else {
        _Impl._Move = _Delegate_move_small<_Vt>;
      }

      if constexpr (_STD is_trivially_destructible_v<_Vt>) {
        _Impl._Destroy = nullptr;
      } else {
        _Impl._Destroy = _Delegate_destroy_small<_Vt>;
      }
    }
    return _Impl;
  }

  template <class _Vt, class _VtInvQuals>
  [[nodiscard]] static const _Impl_t* _Create_impl_ptr() noexcept {
    static constexpr _Impl_t _Impl = _Create_impl<_Vt, _VtInvQuals>();
    return &_Impl;
  }
};

template <class... _Signature>
class _Move_only_Delegate_call {
  static_assert((_STD _Always_false<_Signature> || ...),
                "std::move_only_Delegate only accepts Delegate types as "
                "template arguments, "
                "with possibly const/ref/noexcept qualifiers.");

  static_assert(sizeof...(_Signature) > 0,
                "Unlike std::Delegate, std::move_only_Delegate does not define "
                "class template argument deduction guides.");
};

// A script to generate the specializations is at
// /tools/scripts/move_only_Delegate_specializations.py
// (Avoiding C++ preprocessor for better IDE navigation and debugging
// experience)

template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...)>
    : public _Move_only_Delegate_base<_Rx, false, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = _Vt&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_invocable_r_v<_Rx, _Vt, _Types...> &&
      _STD is_invocable_r_v<_Rx, _Vt&, _Types...>;

  _Rx operator()(_Types... _Args) {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};

template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...)&>
    : public _Move_only_Delegate_base<_Rx, false, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = _Vt&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_invocable_r_v<_Rx, _Vt&, _Types...>;

  _Rx operator()(_Types... _Args) & {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};

template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...) &&>
    : public _Move_only_Delegate_base<_Rx, false, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = _Vt&&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_invocable_r_v<_Rx, _Vt, _Types...>;

  _Rx operator()(_Types... _Args) && {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};

template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...) const>
    : public _Move_only_Delegate_base<_Rx, false, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = const _Vt&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_invocable_r_v<_Rx, const _Vt, _Types...> &&
      _STD is_invocable_r_v<_Rx, const _Vt&, _Types...>;

  _Rx operator()(_Types... _Args) const {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};

template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...) const&>
    : public _Move_only_Delegate_base<_Rx, false, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = const _Vt&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_invocable_r_v<_Rx, const _Vt&, _Types...>;

  _Rx operator()(_Types... _Args) const& {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};

template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...) const&&>
    : public _Move_only_Delegate_base<_Rx, false, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = const _Vt&&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_invocable_r_v<_Rx, const _Vt, _Types...>;

  _Rx operator()(_Types... _Args) const&& {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};

#ifdef __cpp_noexcept_Delegate_type
template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...) noexcept>
    : public _Move_only_Delegate_base<_Rx, true, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = _Vt&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_nothrow_invocable_r_v<_Rx, _Vt, _Types...> &&
      _STD is_nothrow_invocable_r_v<_Rx, _Vt&, _Types...>;

  _Rx operator()(_Types... _Args) noexcept {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};

template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...) & noexcept>
    : public _Move_only_Delegate_base<_Rx, true, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = _Vt&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_nothrow_invocable_r_v<_Rx, _Vt&, _Types...>;

  _Rx operator()(_Types... _Args) & noexcept {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};

template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...) && noexcept>
    : public _Move_only_Delegate_base<_Rx, true, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = _Vt&&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_nothrow_invocable_r_v<_Rx, _Vt, _Types...>;

  _Rx operator()(_Types... _Args) && noexcept {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};

template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...) const noexcept>
    : public _Move_only_Delegate_base<_Rx, true, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = const _Vt&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_nothrow_invocable_r_v<_Rx, const _Vt, _Types...> &&
      _STD is_nothrow_invocable_r_v<_Rx, const _Vt&, _Types...>;

  _Rx operator()(_Types... _Args) const noexcept {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};

template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...) const & noexcept>
    : public _Move_only_Delegate_base<_Rx, true, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = const _Vt&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_nothrow_invocable_r_v<_Rx, const _Vt&, _Types...>;

  _Rx operator()(_Types... _Args) const& noexcept {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};

template <class _Rx, class... _Types>
class _Move_only_Delegate_call<_Rx(_Types...) const && noexcept>
    : public _Move_only_Delegate_base<_Rx, true, _Types...> {
 public:
  using result_type = _Rx;

  template <class _Vt>
  using _VtInvQuals = const _Vt&&;

  template <class _Vt>
  static constexpr bool _Is_callable_from =
      _STD is_nothrow_invocable_r_v<_Rx, const _Vt, _Types...>;

  _Rx operator()(_Types... _Args) const&& noexcept {
    return this->_Get_invoke()(this->_Data, _STD forward<_Types>(_Args)...);
  }
};
#endif  // __cpp_noexcept_Delegate_type

EDCPP_EXPORT template <class... _Signature>
class move_only_Delegate : private _Move_only_Delegate_call<_Signature...> {
 private:
  using _Call = _Move_only_Delegate_call<_Signature...>;

  // clang-format off
    template <class _Fn>
    static constexpr bool _Enable_one_arg_constructor =
        !_STD is_same_v<_STD remove_cvref_t<_Fn>, move_only_Delegate>
        && !_STD _Is_specialization_v<_STD remove_cvref_t<_Fn>, _STD in_place_type_t>
        && _Call::template _Is_callable_from<_STD decay_t<_Fn>>;

    template <class _Fn, class... _CTypes>
    static constexpr bool _Enable_in_place_constructor =
        _STD is_constructible_v<_STD decay_t<_Fn>, _CTypes...>
        && _Call::template _Is_callable_from<_STD decay_t<_Fn>>;

    template <class _Fn, class _Ux, class... _CTypes>
    static constexpr bool _Enable_in_place_list_constructor =
        _STD is_constructible_v<_STD decay_t<_Fn>, _STD initializer_list<_Ux>&, _CTypes...>
        && _Call::template _Is_callable_from<_STD decay_t<_Fn>>;
  // clang-format on
 public:
  using typename _Call::result_type;

  move_only_Delegate() noexcept { this->_Construct_with_null(); }

  move_only_Delegate(nullptr_t) noexcept { this->_Construct_with_null(); }

  move_only_Delegate(move_only_Delegate&&) noexcept = default;

  template <class _Fn,
            _STD enable_if_t<_Enable_one_arg_constructor<_Fn>, int> = 0>
  move_only_Delegate(_Fn&& _Callable) {
    using _Vt = _STD decay_t<_Fn>;
    static_assert(_STD is_constructible_v<_Vt, _Fn>,
                  "_Vt should be constructible from _Fn. "
                  "(N4950 [func.wrap.move.ctor]/6)");

    if constexpr (_STD is_member_pointer_v<_Vt> || _STD is_pointer_v<_Vt> ||
                  _STD _Is_specialization_v<_Vt, move_only_Delegate>) {
      if (_Callable == nullptr) {
        this->_Construct_with_null();
        return;
      }
    }

    using _VtInvQuals = typename _Call::template _VtInvQuals<_Vt>;
    this->template _Construct_with_fn<_Vt, _VtInvQuals>(
        _STD forward<_Fn>(_Callable));
  }

  template <
      class _Fn,
      class... _CTypes,
      _STD enable_if_t<_Enable_in_place_constructor<_Fn, _CTypes...>, int> = 0>
  explicit move_only_Delegate(_STD in_place_type_t<_Fn>, _CTypes&&... _Args) {
    using _Vt = _STD decay_t<_Fn>;
    static_assert(
        _STD is_same_v<_Vt, _Fn>,
        "_Vt should be the same type as _Fn. (N4950 [func.wrap.move.ctor]/12)");

    using _VtInvQuals = typename _Call::template _VtInvQuals<_Vt>;
    this->template _Construct_with_fn<_Vt, _VtInvQuals>(
        _STD forward<_CTypes>(_Args)...);
  }

  template <
      class _Fn,
      class _Ux,
      class... _CTypes,
      _STD enable_if_t<_Enable_in_place_list_constructor<_Fn, _Ux, _CTypes...>,
                       int> = 0>
  explicit move_only_Delegate(_STD in_place_type_t<_Fn>,
                              _STD initializer_list<_Ux> _Li,
                              _CTypes&&... _Args) {
    using _Vt = _STD decay_t<_Fn>;
    static_assert(
        _STD is_same_v<_Vt, _Fn>,
        "_Vt should be the same type as _Fn. (N4950 [func.wrap.move.ctor]/18)");

    using _VtInvQuals = typename _Call::template _VtInvQuals<_Vt>;
    this->template _Construct_with_fn<_Vt, _VtInvQuals>(
        _Li, _STD forward<_CTypes>(_Args)...);
  }

  ~move_only_Delegate() {
    // Do cleanup in this class destructor rather than base,
    // so that if object construction throws, the unnecessary cleanup isn't
    // called.
    this->_Checked_destroy(this->_Data);
  }

  move_only_Delegate& operator=(nullptr_t) noexcept {
    this->_Checked_destroy(this->_Data);
    this->_Reset_to_null();
    return *this;
  }

  move_only_Delegate& operator=(move_only_Delegate&& _Other)
  // intentionally noexcept(false), leaving the door open to adding allocator
  // support in the future; see GH-2278
  {
    if (this != _STD addressof(_Other)) {
      this->_Move_assign(_STD move(_Other));
    }
    return *this;
  }

  template <class _Fn,
            _STD enable_if_t<_STD is_constructible_v<move_only_Delegate, _Fn>,
                             int> = 0>
  move_only_Delegate& operator=(_Fn&& _Callable) {
    this->_Move_assign(move_only_Delegate{_STD forward<_Fn>(_Callable)});
    return *this;
  }

  [[nodiscard]] explicit operator bool() const noexcept {
    return !this->_Is_null();
  }

  using _Call::operator();

  void swap(move_only_Delegate& _Other) noexcept { this->_Swap(_Other); }

  friend void swap(move_only_Delegate& _Fn1,
                   move_only_Delegate& _Fn2) noexcept {
    _Fn1._Swap(_Fn2);
  }

  friend bool operator==(const move_only_Delegate& _This,
                         _STD nullptr_t) noexcept {
    return _This._Is_null();
  }
};
#endif  // _HAS_CXX23
EDCPP_END
#endif
