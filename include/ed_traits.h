#ifndef ED_TRAITS_H
#define ED_TRAITS_H
#include <concepts>
#include <type_traits>
#include "ed_base.h"
#include "senders/default_sender.h"
EDCPP_BEGIN
/// For use with disjunction and conjunction
///
/// @returns true if and only if V is true with the type of T
template <bool V, typename T>
struct value_true : std::bool_constant<V> {
  using type = T;
};
/// For use with disjunction and conjunction
///
/// @static constexpr that always returns true along with its type
template <typename T>
struct default_type : std::true_type {
  using type = T;
};
/// Returns the function type given the template parameters
///
/// Finds the function type through a disjunction, or of all the
/// major function types. The disjunction is a series of value_true followed by
/// a default_type.
///
/// @since 0.0.2
///
/// @tparam RC The return type of the function
/// @tparam _noexcept true if the function is noexcept, false if not
/// @tparam _const true if the function is const, false if not
/// @tparam _ampersand true if the function is passed by reference, false if it
/// is not passed by reference
/// @tparam _ampersand_ampersand true if the function is passed by rvalue
/// reference (two ampersands), false if not
/// @tparam Args The arguments of the function
///
/// @note A second using had to be added to convert the function
/// type into a function pointer because the compiler was not able
/// to handle it as a single using.
///
/// @todo Need to understand the reference qualifiers better and what to do with
/// them.
///
/// @see default_type
/// @see value_true
template <class RC,
          bool _noexcept,
          bool _const,
          bool _ampersand,
          bool _ampersand_ampersand,
          class... Args>
struct func_y {
  /// The function type
  using FT = typename _STD disjunction<
      value_true<(_noexcept && _const && (!_ampersand) && _ampersand_ampersand),
                 RC(Args...) const && noexcept>,
      value_true<(_noexcept && _const && _ampersand && (!_ampersand_ampersand)),
                 RC(Args...) const & noexcept>,
      value_true<(_noexcept && _const && (!_ampersand) &&
                  (!_ampersand_ampersand)),
                 RC(Args...) const noexcept>,
      value_true<((!_noexcept) && _const && (!_ampersand) &&
                  _ampersand_ampersand),
                 RC(Args...) const&&>,
      value_true<((!_noexcept) && _const && _ampersand &&
                  (!_ampersand_ampersand)),
                 RC(Args...) const&>,
      value_true<((!_noexcept) && _const && (!_ampersand) &&
                  (!_ampersand_ampersand)),
                 RC(Args...) const>,
      value_true<(_noexcept && (!_const) && (!_ampersand) &&
                  _ampersand_ampersand),
                 RC(Args...) && noexcept>,
      value_true<(_noexcept && (!_const) && _ampersand &&
                  (!_ampersand_ampersand)),
                 RC(Args...) & noexcept>,
      value_true<(_noexcept && (!_const) && (!_ampersand) &&
                  (!_ampersand_ampersand)),
                 RC(Args...) noexcept>,
      default_type<RC(Args...)>>::type;

  /// The function pointer type associated with the function type give by FT
  using FP = typename _STD add_pointer<FT>::type;
};
/// A default container for type traits of functions supported as delegates.
///
/// @note that volatile is not supported for delegate such that volatile
/// delegates will end up in this bucket.
template <class C>
struct func_y_traits {};
/// A basic function type with no following qualifiers
template <class RC, class... Args>
struct func_y_traits<RC(Args...)>
    : func_y<RC, false, false, false, false, Args...> {};

/// A basic function type with noexcept
template <class RC, class... Args>
struct func_y_traits<RC(Args...) noexcept> {};

/// A basic function type with noexcept and ampersand
template <class RC, class... Args>
struct func_y_traits<RC(Args...) & noexcept> {};

/// A basic function type with noexcept and ampersand-ampersand
template <class RC, class... Args>
struct func_y_traits<RC(Args...) && noexcept> {};

/// A basic function type with const
template <class RC, class... Args>
struct func_y_traits<RC(Args...) const> {};

/// A basic function type with const ampersand
template <class RC, class... Args>
struct func_y_traits<RC(Args...) const&> {};

/// A basic function type with const ampersand-ampersand
template <class RC, class... Args>
struct func_y_traits<RC(Args...) const&&> {};

/// A basic function type with const noexcept
template <class RC, class... Args>
struct func_y_traits<RC(Args...) const noexcept> {};

/// A basic function type with const noexcept and ampersand
template <class RC, class... Args>
struct func_y_traits<RC(Args...) const & noexcept> {};

/// A basic function type with const noexcept and ampersand-ampersand
template <class RC, class... Args>
struct func_y_traits<RC(Args...) const && noexcept> {};

/// @brief The most efficient delegate.
EDCPP_EXPORT using DefaultDelegate = void(Info const&) noexcept;

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
