#ifndef ED_TRAITS_H
#define ED_TRAITS_H
#include <concepts>
#include <type_traits>
#include "ed_base.h"
#include "senders/default_sender.h"
EDCPP_BEGIN
/// @brief Allow return of a type that is an alias for the partial type void but
/// can passed as a type.
///
/// @details The main reason for this type is to allow Google Test to use a
/// param that tells a Google type test that it is working with a void return
/// type. It is just as valid a way to specialize a template as using void.
class TheVoidType {};
template <bool V, typename T>
struct value_true : std::bool_constant<V> {};
/// For use with disjunction and conjunction
///
/// @returns true if and only if V is true with the type of T
template <bool V, typename T>
struct value_type_true : std::bool_constant<V> {
  using type = T;
};
/// For use with disjunction and conjunction
///
/// @static constexpr that always returns true along with its type
template <typename T>
struct default_type : std::true_type {
  using type = T;
};
/// @brief The most efficient delegate.
EDCPP_EXPORT using DefaultDelegate = void(Info const&) noexcept;
/// @brief Allow access to function trait values
///
/// @since 0.0.2
///
/// @tparam _fyt_NOEXCEPT true if the function is noexcept, false if not
/// @tparam _fyt_CONST true if the function is const, false if not
template <bool _fyt_NOEXCEPT, bool _fyt_CONST>
struct func_yield_trait_values {
  /// The exception regime noexcept or not
  static constexpr bool noexcept_v = _fyt_NOEXCEPT;
  /// The constness of the function
  ///
  /// @return true if the function is const
  /// @return true if the function is not declared const
  static constexpr bool const_v = _fyt_CONST;
};
/// @brief Internal function that returns the function type given the template
/// parameters
///
/// @details
/// Updates to this class require updates to \ref A_TT01 Typed Test TT01 in
/// file ConceptsUnitGTest.cpp.
///
/// Finds the function type through a disjunction, or of all the
/// major function types. The std disjunction is a series of value_type_true
/// followed by a default_type.
///
/// func_yield_traits does all the dirty work of figuring out the function type
/// and kinds of information available to the event and delegate objects. The
/// func_y_traits or any template that can break down syntax of the function and
/// pass the individuals bits can use or inherit from func_yield_traits.
///
/// Assumption: This template is never called with an unsupported function type.
/// The calling template should act as a filter.
///
/// @since 0.0.2
///
/// @tparam _fyt_RETURN_CODE The return type of the function
/// @tparam _fyt_NOEXCEPT true if the function is noexcept, false if not
/// @tparam _fyt_CONST true if the function is const, false if not
/// @tparam _fyt_FUNCTION_PARAM_LIST The arguments of the function
///
/// @note A second using had to be added to convert the function
/// type into a function pointer because the compiler was not able
/// to handle it as a single using.
///
/// @todo Need to understand the reference qualifiers better and what to do with
/// them.
///
/// @see default_type
/// @see value_type_true
template <class _fyt_RETURN_CODE,
          bool _fyt_NOEXCEPT,
          bool _fyt_CONST,
          class... _fyt_FUNCTION_PARAM_LIST>
struct func_yield_traits
    : public func_yield_trait_values<_fyt_NOEXCEPT, _fyt_CONST> {
  /// Determines the function type based on the template parameters
  using FunctionType_t = typename _STD disjunction<
      // R(Args ...) noexcept const
      value_type_true<(_fyt_NOEXCEPT && _fyt_CONST),
                      _fyt_RETURN_CODE(_fyt_FUNCTION_PARAM_LIST...)
                          const noexcept>,
      // R(Args ...) const
      value_type_true<((!_fyt_NOEXCEPT) && _fyt_CONST),
                      _fyt_RETURN_CODE(_fyt_FUNCTION_PARAM_LIST...) const>,
      // R(Args ...) noexcept
      value_type_true<(_fyt_NOEXCEPT && (!_fyt_CONST)),
                      _fyt_RETURN_CODE(_fyt_FUNCTION_PARAM_LIST...) noexcept>,
      // R(Args ...)
      value_type_true<((!_fyt_NOEXCEPT) && (!_fyt_CONST)),
                      _fyt_RETURN_CODE(_fyt_FUNCTION_PARAM_LIST...)>>::type;

  /// The function pointer type associated with the function type give by FT
  using FunctionPointer_t = typename _STD add_pointer<FunctionType_t>::type;
  /// The return code type
  using ReturnCode_t = _fyt_RETURN_CODE;
  template <size_t N>
  using NthParam_t = typename _STD
      tuple_element<N, _STD tuple<_fyt_FUNCTION_PARAM_LIST...>>::type;
  /// The number of arguments specified as a variadic template parameter
  ///
  /// This looks complicated because void can be a parameter type but void means
  /// there are no arguments. Unfortunately, specifying void counts as an
  /// argument as far a std::tuple and sizeof are concerned. So, we return 0
  /// arguments in the case where void is specified as the first argument.
  ///
  /// @returns count of arguments in the function definition.
  static constexpr size_t function_param_count_v =
      _STD tuple_size_v<_STD tuple<_fyt_FUNCTION_PARAM_LIST...>>;
  static constexpr bool is_default_delegate_v =
      _STD is_same_v<FunctionType_t, DefaultDelegate>;
  static constexpr bool is_standard_delegate_v =
      function_param_count_v == 1 &&
      (is_default_delegate_v || _STD is_base_of_v<Info, NthParam_t<0>>);
};
template <bool _fyt_NOEXCEPT,
          bool _fyt_CONST,
          class... _fyt_FUNCTION_PARAM_LIST>
struct func_yield_traits<TheVoidType,
                         _fyt_NOEXCEPT,
                         _fyt_CONST,
                         _fyt_FUNCTION_PARAM_LIST...>
    : public func_yield_trait_values<_fyt_NOEXCEPT, _fyt_CONST> {
  /// Determines the function type based on the template parameters
  using FunctionType_t = typename _STD disjunction<
      // R(Args ...) noexcept const
      value_type_true<(_fyt_NOEXCEPT && _fyt_CONST),
                      void(_fyt_FUNCTION_PARAM_LIST...) const noexcept>,
      // R(Args ...) const
      value_type_true<((!_fyt_NOEXCEPT) && _fyt_CONST),
                      void(_fyt_FUNCTION_PARAM_LIST...) const>,
      // R(Args ...) noexcept
      value_type_true<(_fyt_NOEXCEPT && (!_fyt_CONST)),
                      void(_fyt_FUNCTION_PARAM_LIST...) noexcept>,
      // R(Args ...)
      value_type_true<((!_fyt_NOEXCEPT) && (!_fyt_CONST)),
                      void(_fyt_FUNCTION_PARAM_LIST...)>>::type;

  /// The function pointer type associated with the function type give by FT
  using FunctionPointer_t = typename _STD add_pointer<FunctionType_t>::type;
  /// The return code type
  using ReturnCode_t = TheVoidType;
  template <size_t N>
  using NthParam_t = typename _STD
      tuple_element<N, _STD tuple<_fyt_FUNCTION_PARAM_LIST...>>::type;
  /// The number of arguments specified as a variadic template parameter
  ///
  /// This looks complicated because void can be a parameter type but void means
  /// there are no arguments. Unfortunately, specifying void counts as an
  /// argument as far a std::tuple and sizeof are concerned. So, we return 0
  /// arguments in the case where void is specified as the first argument.
  ///
  /// @returns count of arguments in the function definition.
  static constexpr size_t function_param_count_v =
      _STD tuple_size_v<_STD tuple<_fyt_FUNCTION_PARAM_LIST...>>;
  static constexpr bool is_default_delegate_v =
      _STD is_same_v<FunctionType_t, DefaultDelegate>;
  static constexpr bool is_standard_delegate_v =
      function_param_count_v == 1 &&
      (is_default_delegate_v || _STD is_base_of_v<Info, NthParam_t<0>>);
};
/// @brief Internal function that returns the function type given the template
/// parameters
///
/// @details
/// Updates to this class require updates to \ref A_TT01 Typed Test TT01 in
/// file ConceptsUnitGTest.cpp.
///
/// Finds the function type through a disjunction, or of all the
/// major function types. The std disjunction is a series of value_type_true
/// followed by a default_type.
///
/// func_yield_traits does all the dirty work of figuring out the function type
/// and kinds of information available to the event and delegate objects. The
/// func_y_traits or any template that can break down syntax of the function and
/// pass the individuals bits can use or inherit from func_yield_traits.
///
/// Assumption: This template is never called with an unsupported function type.
/// The calling template should act as a filter.
///
/// @since 0.0.2
///
/// @tparam _fyt_RETURN_CODE The return type of the function
/// @tparam _fyt_NOEXCEPT true if the function is noexcept, false if not
/// @tparam _fyt_CONST true if the function is const, false if not
///
/// @note A second using had to be added to convert the function
/// type into a function pointer because the compiler was not able
/// to handle it as a single using.
///
/// @see default_type
/// @see value_type_true
template <class _fyt_RETURN_CODE, bool _fyt_NOEXCEPT, bool _fyt_CONST>
struct func_yield_traits<_fyt_RETURN_CODE,
                         _fyt_NOEXCEPT,
                         _fyt_CONST,
                         TheVoidType>
    : public func_yield_trait_values<_fyt_NOEXCEPT, _fyt_CONST> {
  /// Determines the function type based on the template parameters
  /// @todo fix the problem with the parameter list void creating func(void)
  using FunctionType_t = typename _STD disjunction<
      // R() noexcept const
      value_type_true<(_fyt_NOEXCEPT && _fyt_CONST),
                      _fyt_RETURN_CODE() const noexcept>,
      // R() const
      value_type_true<((!_fyt_NOEXCEPT) && _fyt_CONST),
                      _fyt_RETURN_CODE() const>,
      // R() noexcept
      value_type_true<(_fyt_NOEXCEPT && (!_fyt_CONST)),
                      _fyt_RETURN_CODE() noexcept>,
      // R()
      value_type_true<((!_fyt_NOEXCEPT) && (!_fyt_CONST)),
                      _fyt_RETURN_CODE()>>::type;

  /// The function pointer type associated with the function type give by FT
  using FunctionPointer_t = typename _STD add_pointer<FunctionType_t>::type;
  /// The return code type
  using ReturnCode_t = _fyt_RETURN_CODE;
  /// The number of arguments specified as a variadic template parameter
  ///
  /// This looks complicated because void can be a parameter type but void means
  /// there are no arguments. Unfortunately, specifying void counts as an
  /// argument as far a std::tuple and sizeof are concerned. So, we return 0
  /// arguments in the case where void is specified as the first argument.
  ///
  /// @returns count of arguments in the function definition.
  static constexpr size_t function_param_count_v = 0;
  static constexpr bool is_default_delegate_v = false;
  static constexpr bool is_standard_delegate_v = false;
};
template <bool _fyt_NOEXCEPT, bool _fyt_CONST>
struct func_yield_traits<TheVoidType, _fyt_NOEXCEPT, _fyt_CONST, TheVoidType>
    : public func_yield_trait_values<_fyt_NOEXCEPT, _fyt_CONST> {
  /// Determines the function type based on the template parameters
  /// @todo fix the problem with the parameter list void creating func(void)
  using FunctionType_t = typename _STD disjunction<
      // R() noexcept const
      value_type_true<(_fyt_NOEXCEPT && _fyt_CONST), void() const noexcept>,
      // R() const
      value_type_true<((!_fyt_NOEXCEPT) && _fyt_CONST), void() const>,
      // R() noexcept
      value_type_true<(_fyt_NOEXCEPT && (!_fyt_CONST)), void() noexcept>,
      // R()
      value_type_true<((!_fyt_NOEXCEPT) && (!_fyt_CONST)), void()>>::type;

  /// The function pointer type associated with the function type give by FT
  using FunctionPointer_t = typename _STD add_pointer<FunctionType_t>::type;
  /// The return code type
  using ReturnCode_t = TheVoidType;
  /// The number of arguments specified as a variadic template parameter
  ///
  /// This looks complicated because void can be a parameter type but void means
  /// there are no arguments. Unfortunately, specifying void counts as an
  /// argument as far a std::tuple and sizeof are concerned. So, we return 0
  /// arguments in the case where void is specified as the first argument.
  ///
  /// @returns count of arguments in the function definition.
  static constexpr size_t function_param_count_v = 0;
  static constexpr bool is_default_delegate_v = false;
  static constexpr bool is_standard_delegate_v = false;
};
/// A default container for type traits of functions supported as delegates.
///
/// @note that volatile is not supported for delegate such that volatile
/// delegates will end up in this bucket.
template <class C>
struct func_y_traits {};
/// A basic function type with parameters and no following qualifiers
template <class ReturnCode, class... FunctionParamList>
struct func_y_traits<ReturnCode(FunctionParamList...)>
    : func_yield_traits<ReturnCode,           // return type
                        false,                // noexcept
                        false,                // const
                        FunctionParamList...  // function arguments
                        > {};
/// A basic function type with no parameters and no following qualifiers
template <class ReturnCode>
struct func_y_traits<ReturnCode()>
    : func_yield_traits<ReturnCode,  // return type
                        false,       // noexcept
                        false        // const
                        > {};

/// A basic function type with noexcept
template <class ReturnCode, class... FunctionParamList>
struct func_y_traits<ReturnCode(FunctionParamList...) noexcept>
    : func_yield_traits<ReturnCode,           // return type
                        true,                 // noexcept
                        false,                // const
                        FunctionParamList...  // function arguments
                        > {};

/// A basic function type with const
template <class ReturnCode, class... FunctionParamList>
struct func_y_traits<ReturnCode(FunctionParamList...) const>
    : func_yield_traits<ReturnCode,           // return type
                        false,                // noexcept
                        true,                 // const
                        FunctionParamList...  // function arguments
                        > {};

/// A basic function type with const noexcept
template <class ReturnCode, class... FunctionParamList>
struct func_y_traits<ReturnCode(FunctionParamList...) const noexcept>
    : func_yield_traits<ReturnCode,           // return type
                        true,                 // noexcept
                        true,                 // const
                        FunctionParamList...  // function arguments
                        > {};

/// The defaulting DefaultDelegate check
///
/// @tparam I Anything but _EDCPP Info.
EDCPP_EXPORT template <class>
struct is_default_delegate : std::false_type {};

/// @brief The money shot, the most efficient Delegate
///
/// @details Passing this test does not mean the function can be used as a
/// DefaultDelegate, the concept IsDefaultDelegate will implement other tests.
EDCPP_EXPORT template <class I>
  requires _STD
same_as<I, DefaultDelegate> struct is_default_delegate<I> : _STD true_type {};

/// @brief The value of the boolean answer to is_default_delegate<func>::value.
///
/// @tparam I Anything but _EDCPP Info.
///
/// @return true if class I is a DefaultDelegate
EDCPP_EXPORT template <class I>
inline constexpr bool is_default_delegate_v = is_default_delegate<I>::value;

template <class>
struct is_standard_delegate : _STD false_type {};

template <class I>
inline constexpr bool is_standard_delegate_v = is_standard_delegate<I>::value;

EDCPP_END
#endif
