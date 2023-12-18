#ifndef ED_TRAITS_H
#define ED_TRAITS_H
/*
    EventDelegateCPP is a code library that provide event delegate
        functionality to C++

    Copyright (C) 2023,2024  Jack D. Menendez

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
/// @file ed_traits.h
/// @brief Traits of functions for events and delegates
///
/// @ingroup eventdelegatecpp
/// @anchor ed_traits
///
/// @details
/// A delegate needs to know the characteristics of the function it is expected
/// to encapsulate and share to an event.
///
/// @anchor function_equivalence
///
/// Function Equivalence
/// --------------------
/// For the purposes of this event-delegate model, two or more functions are
/// equivalent if they have the same return value, parameter list, const and
/// exception regime. This means that a function from two different classes that
/// have the same return value, parameters, const, and exception regime are
/// equivalent. Furthermore, in the eyes of a Delegate and Event, two functions
/// are equivalent even if one is a class method and the other is static or
/// normal procedure as long the return value, parameters, and exception regime
/// are the same.
///
/// @copyright 2023,2024 Jack D Menendez
/// @author Jack D. Menendez
/// @date 01/09/2023
/// @since 01/09/2023
/// @version 0.0.1
///
#include <concepts>
#include <string>
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
/// @brief Comparable values for use with unit tests and disjunctions to test
/// which specialization was used.
/// @details The values are not important, only that they are different.
/// The values do not determine specialization but specialization determines the
/// value used.
enum class func_trait_specialization {
  GENERAL = 0,         ///< The default func_yield_traits
  RC_PARAMS_EXCEPT,  ///< func_yield_traits with return code and parameters
                       ///< and exception regime.
  VOID_PARAMS_EXCEPT,  ///< func_yield_traits with void return, parameters, and
                       ///< exception regime.
  RC__EXCEPT,   ///< func_yield_traits with return code, no parameters, and
                ///< exception regime
  VOID__EXCEPT  ///< func_yield_traits with void return, no parameters, and
                ///< exception regime
};
/// @brief The most efficient delegate.
EDCPP_EXPORT using DefaultDelegate = void(Info const&) noexcept;
/// @brief Allow access to function trait values
/// @details Designed to be inherited by other classes that need access to these
/// traits.
/// @since 0.0.2
/// @tparam _fyt_NOEXCEPT true if the function is noexcept, false if not
/// @tparam _fyt_CONST true if the function is const, false if no
/// @tparam _fyt_SPECIALIZATION_ID a description of the specialization of
/// func_yield_traits
/// @tparam _fyt_SPECIALIZATION A comparable value for use with unit tests that
/// can be checked to see if the expected specialization occurred.
template <bool _fyt_NOEXCEPT,
          bool _fyt_CONST,
          StringTParm _fyt_SPECIALIZATION_ID,
          func_trait_specialization _fyt_SPECIALIZATION>
struct func_yield_trait_values {
  /// The exception regime noexcept or not
  static constexpr bool noexcept_v = _fyt_NOEXCEPT;
  /// The specialization of func_yield_traits that was actually inferred by the
  /// compiler
  static constexpr int specialization_v = static_cast<int>(_fyt_SPECIALIZATION);
  /// The constness of the function
  ///
  /// @return true if the function is const
  /// @return false if the function is not declared const
  static constexpr bool const_v = _fyt_CONST;
  static constexpr auto specialization_id_v = _fyt_SPECIALIZATION_ID.value;
};
/// @brief The general case for function traits that reflects various aspects of
/// a function and the function type
/// @details This class allows us to build a container for a function.
/// @tparam _fyt_RETURN_CODE The return type of the function
/// @tparam _fyt_OBJECT the object type owning the method.
/// @tparam _fyt_NOEXCEPT true if the function is noexcept, false if not
/// @tparam _fyt_CONST true if the function is const, false if not
/// @tparam _fyt_FUNCTION_PARAM_LIST The arguments of the function or method
/// @note Tried inheriting from func_yield but the compiler was not able to
/// instantiate the type in the order we need.
/// @todo move func_yild_traits_value from inherited to member.
template <class _fyt_RETURN_CODE,
          class _fyt_OBJECT,
          bool _fyt_NOEXCEPT,
          bool _fyt_CONST,
          class... _fyt_FUNCTION_PARAM_LIST>
struct func_yield_traits
    : public func_yield_trait_values<_fyt_NOEXCEPT,
                                     _fyt_CONST,
                                     "No specialization",
                                     func_trait_specialization::GENERAL> {};
/// @brief A partial specialization for functions that are not non-static
/// members of a class.
///
/// @details
/// This specialization is instantiated when fyt_const is false and fyt_object
/// is TheVoidType Updates to this class require updates to \ref A_TT01 Typed
/// Test TT01 in file ConceptsUnitGTest.cpp.
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
/// @tparam _fyt_OBJECT is TheVoidType the object type owning the method.
/// @tparam _fyt_NOEXCEPT true if the function is noexcept, false if not
/// @tparam _fyt_CONST is false
/// @tparam _fyt_FUNCTION_PARAM_LIST The arguments of the function
///
/// @note A second using had to be added to convert the function
/// type into a function pointer because the compiler was not able
/// to handle it as a single using.
///
/// @see default_type
/// @see value_type_true
template <class _fyt_RETURN_CODE,
          bool _fyt_NOEXCEPT,
          class... _fyt_FUNCTION_PARAM_LIST>
struct func_yield_traits<_fyt_RETURN_CODE,
                         TheVoidType,  // No real object type specialization
                         _fyt_NOEXCEPT,
                         false,  // const specialization
                         _fyt_FUNCTION_PARAM_LIST...>
    : public func_yield_trait_values<
          _fyt_NOEXCEPT,
          false,
          "RC( params...)",
          func_trait_specialization::RC_PARAMS_EXCEPT> {
  /// Determines the function type based on the template parameters
  using FunctionType_t = typename _STD disjunction<
      // R(Args ...) noexcept
      value_type_true<(_fyt_NOEXCEPT),
                      _fyt_RETURN_CODE(_fyt_FUNCTION_PARAM_LIST...) noexcept>,
      // R(Args ...)
      value_type_true<(!_fyt_NOEXCEPT),
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
template <bool _fyt_NOEXCEPT, class... _fyt_FUNCTION_PARAM_LIST>
struct func_yield_traits<TheVoidType,
                         TheVoidType,
                         _fyt_NOEXCEPT,
                         false,
                         _fyt_FUNCTION_PARAM_LIST...>
    : public func_yield_trait_values<
          _fyt_NOEXCEPT,
          false,
          "void(params...)",
          func_trait_specialization::VOID_PARAMS_EXCEPT> {
  /// Determines the function type based on the template parameters
  using FunctionType_t = typename _STD disjunction<
      // R(Args ...) noexcept
      value_type_true<(_fyt_NOEXCEPT),
                      void(_fyt_FUNCTION_PARAM_LIST...) noexcept>,
      // R(Args ...)
      value_type_true<(!_fyt_NOEXCEPT),
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
template <class _fyt_RETURN_CODE, bool _fyt_NOEXCEPT>
struct func_yield_traits<_fyt_RETURN_CODE,
                         TheVoidType,
                         _fyt_NOEXCEPT,
                         false,
                         TheVoidType>
    : public func_yield_trait_values<_fyt_NOEXCEPT,
                                     false,
                                     "RC()",
                                     func_trait_specialization::RC__EXCEPT> {
  /// Determines the function type based on the template parameters
  /// @todo fix the problem with the parameter list void creating func(void)
  using FunctionType_t = typename _STD disjunction<
      // R() noexcept
      value_type_true<_fyt_NOEXCEPT, _fyt_RETURN_CODE() noexcept>,
      // R()

      value_type_true<!_fyt_NOEXCEPT, _fyt_RETURN_CODE()>>::type;

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
template <bool _fyt_NOEXCEPT>
struct func_yield_traits<TheVoidType,
                         TheVoidType,
                         _fyt_NOEXCEPT,
                         false,
                         TheVoidType>
    : public func_yield_trait_values<_fyt_NOEXCEPT,
                                     false,
                                     "void()",
                                     func_trait_specialization::VOID__EXCEPT> {
  /// Determines the function type based on the template parameters
  /// @todo fix the problem with the parameter list void creating func(void)
  using FunctionType_t = typename _STD disjunction<
      // R() noexcept
      value_type_true<_fyt_NOEXCEPT, void() noexcept>,
      // R()
      value_type_true<!_fyt_NOEXCEPT, void()>>::type;

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
/// A basic function type with no return with parameters and no following
/// qualifiers
template <class... FunctionParamList>
struct func_y_traits<void(FunctionParamList...)>
    : func_yield_traits<void,  // return type
                        TheVoidType,
                        false,                // noexcept
                        false,                // const
                        FunctionParamList...  // function arguments
                        > {};
/// A basic method type with no return with parameters and no following
/// qualifiers
template <class ObjectType, class... FunctionParamList>
struct func_y_traits<void (ObjectType::*)(FunctionParamList...)>
    : func_yield_traits<void,  // return type
                        ObjectType,
                        false,                // noexcept
                        false,                // const
                        FunctionParamList...  // function arguments
                        > {};
/// A basic function type with parameters and no following qualifiers
template <class ReturnCode, class... FunctionParamList>
struct func_y_traits<ReturnCode(FunctionParamList...)>
    : func_yield_traits<ReturnCode,  // return type
                        TheVoidType,
                        false,                // noexcept
                        false,                // const
                        FunctionParamList...  // function arguments
                        > {};
template <class ReturnCode, class ObjectType, class... FunctionParamList>
struct func_y_traits<ReturnCode (ObjectType::*)(FunctionParamList...)>
    : func_yield_traits<ReturnCode,  // return type
                        ObjectType,
                        false,                // noexcept
                        false,                // const
                        FunctionParamList...  // function arguments
                        > {};
/// A basic function type with no parameters and no following qualifiers
template <class ReturnCode>
struct func_y_traits<ReturnCode()>
    : func_yield_traits<ReturnCode,  // return type
                        TheVoidType,
                        false,  // noexcept
                        false   // const
                        > {};
/// A basic method type with no parameters and no following qualifiers
template <class ReturnCode, class ObjectType>
struct func_y_traits<ReturnCode (ObjectType::*)()>
    : func_yield_traits<ReturnCode,  // return type
                        ObjectType,
                        false,  // noexcept
                        false   // const
                        > {};

/// A basic function type with noexcept
template <class ReturnCode, class... FunctionParamList>
struct func_y_traits<ReturnCode(FunctionParamList...) noexcept>
    : func_yield_traits<ReturnCode,  // return type
                        TheVoidType,
                        true,                 // noexcept
                        false,                // const
                        FunctionParamList...  // function arguments
                        > {};
/// A basic method type with noexcept.
template <class ReturnCode, class ObjectType, class... FunctionParamList>
struct func_y_traits<ReturnCode (ObjectType::*)(FunctionParamList...) noexcept>
    : func_yield_traits<ReturnCode,  // return type
                        ObjectType,
                        true,                 // noexcept
                        false,                // const
                        FunctionParamList...  // function arguments
                        > {};

/// A method type with const
template <class ReturnCode, class ObjectType, class... FunctionParamList>
struct func_y_traits<ReturnCode (ObjectType::*)(FunctionParamList...) const>
    : func_yield_traits<ReturnCode,  // return type
                        ObjectType,
                        false,                // noexcept
                        true,                 // const
                        FunctionParamList...  // function arguments
                        > {};

/// A basic function type with const noexcept
template <class ReturnCode, class ObjectType, class... FunctionParamList>
struct func_y_traits<ReturnCode (ObjectType::*)(FunctionParamList...)
                         const noexcept>
    : func_yield_traits<ReturnCode,  // return type
                        ObjectType,
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
