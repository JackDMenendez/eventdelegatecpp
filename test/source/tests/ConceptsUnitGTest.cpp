#include <gtest/gtest.h>
#include <functional>
#include <type_traits>
#define MTESTREGIME 1
#include <concepts>
#include "event_concepts.h"
#include "test_functions.h"
#include "util/ghelper.h"
#include "util/unit_test.h"
// #define TT01 1
#define UT01 1
// #define UT02 2
// #define UT03 3
// #define UT04 4
// #define UT05 5
// #define UT06 6
// #define UT07 7
// #define UT08 8
// #define UT09 9
// #define UT10 10
// #define UT11 11
// #define UT12 12
// #define UT13 13
// #define UT14 14
EDCPP_USING
USING_EDCPP_UNIT_TEST
template <class C>
struct TestFixtureConcept1 : public testing::Test {};
#ifdef TEMP_EXCLUDE
template <class RC, class... Args>
struct TestFixtureConcept1<type_container<void, RC(Args...)>>
    : public testing::Test {
  static void printTypes() {
    TEST_INFO() << "testing RC Returns: ";
    ArgPrinter<RC>::print();
    // print(RC);
    MINFO << "(";
    ArgPrinter<Args...>::print();
    // print(Args...);
    MINFO << ")" << TEST_EOL;
  }
  static RC (*testFunction)(Args... args);
  using F = decltype(testFunction);
  using R = RC;
  static constexpr auto numargs{sizeof...(Args)};
  static constexpr bool is_noexcept = false;
  static constexpr bool is_return_void = false;
  static constexpr bool is_const = false;
  static constexpr bool is_lambda = false;
  static constexpr bool is_function = _STD is_function<F>::value;
  static constexpr bool is_function_pointer = true;
  static constexpr bool is_member_function_pointer = false;
  static constexpr bool is_functor = false;
  static constexpr bool is_invocable = _STD is_invocable<F>::value;
};
template <class RC, class... Args>
struct TestFixtureConcept1<type_container<void, RC(Args...) noexcept>>
    : public testing::Test {
  static void printTypes() {
    TEST_INFO() << "testing RC noexcept Returns: ";
    // print(RC);
    ArgPrinter<RC>::print();
    MINFO << "(";
    ArgPrinter<Args...>::print();
    // print(Args...);
    MINFO << ") noexcept" << TEST_EOL;
  }
  static RC (*testFunction)(Args...) noexcept;
  using F = decltype(testFunction);
  using R = RC;
  static constexpr auto numargs{sizeof...(Args)};
  static constexpr bool is_noexcept = true;
  static constexpr bool is_return_void = false;
  static constexpr bool is_const = false;
  static constexpr bool is_lambda = false;
  static constexpr bool is_function = _STD is_function<F>::value;
  static constexpr bool is_function_pointer = true;
  static constexpr bool is_member_function_pointer = false;
  static constexpr bool is_functor = false;
  static constexpr bool is_invocable = _STD is_invocable<F>::value;
};
/**
 * Accept a non-member function.
 */
template <class... Args>
struct TestFixtureConcept1<type_container<void, void(Args...)>>
    : public testing::Test {
  static void printTypes() {
    TEST_INFO() << "testing void Args: (";
    ArgPrinter<Args...>::print();
    //    print(Args...);
    MINFO << ")" << TEST_EOL;
  }
  static void (*testFunction)(Args...);
  using F = decltype(testFunction);
  using R = void;
  static constexpr auto numargs{sizeof...(Args)};
  static constexpr bool is_noexcept = true;
  static constexpr bool is_return_void = true;
  static constexpr bool is_const = false;
  static constexpr bool is_lambda = false;
  static constexpr bool is_function = _STD is_function<F>::value;
  static constexpr bool is_function_pointer = true;
  static constexpr bool is_member_function_pointer = false;
  static constexpr bool is_functor = false;
  static constexpr bool is_invocable = _STD is_invocable<F>::value;
};

template <class... Args>
struct TestFixtureConcept1<type_container<void, void(Args...) noexcept>>
    : public testing::Test {
  static void printTypes() {
    TEST_INFO() << "testing void noexcept Args: (";
    ArgPrinter<Args...>::print();
    MINFO << ") noexcept" << TEST_EOL;
  }
  static void (*testFunction)(Args...) noexcept;
  using F = decltype(testFunction);
  using R = void;
  static constexpr auto numargs{sizeof...(Args)};
  static constexpr bool is_noexcept = true;
  static constexpr bool is_return_void = true;
  static constexpr bool is_const = false;
  static constexpr bool is_function = _STD is_function<F>::value;
  static constexpr bool is_function_pointer = true;
  static constexpr bool is_member_function_pointer = false;
  static constexpr bool is_functor = false;
  static constexpr bool is_invocable = _STD is_invocable<F>::value;
};
template <>
struct TestFixtureConcept1<type_container<void, void()>>
    : public testing::Test {
  static void printTypes() {
    TEST_INFO() << "testing void  NoArgs: (";
    MINFO << ")" << TEST_EOL;
  }
  static void (*testFunction)();
  using F = decltype(testFunction);
  using R = void;
  static constexpr bool is_noexcept = false;
  static constexpr bool is_return_void = true;
  static constexpr auto numargs{0};
  static constexpr bool is_const = false;
  static constexpr bool is_lambda = false;
  static constexpr bool is_function = _STD is_function_v<F>;
  static constexpr bool is_function_pointer = true;
  static constexpr bool is_member_function_pointer =
      _STD is_member_function_pointer_v<F>;
  static constexpr bool is_functor = false;
  static constexpr bool is_invocable = _STD is_invocable_v<F>;
};
#endif
template <class RC, class... Args>
struct TestFixtureConcept1<type_container<RC, RC(Args...) noexcept>>
    : public testing::Test {
  static void printTypes() {
    TEST_INFO() << "testing noexcept void NoArgs: (";
    ArgPrinter<Args...>::print();
    MINFO << ")" << TEST_EOL;
  }
  static RC (*testFunction)(Args...) noexcept;
  using F = decltype(testFunction);
  using R = RC;
  static constexpr bool is_const = _STD is_const_v<F>;
  static constexpr bool is_except = !_STD is_nothrow_invocable_v<F>;
  static constexpr bool is_function =
      _STD is_function_v<_STD remove_pointer<F>::type>;
  static constexpr bool is_function_pointer =
      is_function && _STD is_pointer_v<F>;
  ;
  static constexpr bool is_functor = false;
  static constexpr bool is_invocable = _STD is_invocable_v<F>;
  static constexpr bool is_lambda = false;
  static constexpr bool is_member_function_pointer =
      _STD is_member_function_pointer_v<F>;
  static constexpr auto numargs{sizeof...(Args)};
  static constexpr bool is_noexcept = !is_except;
  static constexpr bool is_return_void = _STD is_same_v<R, void>;
};
VOID_L_EXCEPT;
VOID_L_NOEXCEPT;

using void_function_void = type_container<void, decltype(void_f_except)>;
using TestFixture_void_function_void = TestFixtureConcept1<void_function_void>;
using void_function_void_noexcept =
    type_container<void, decltype(void_f_noexcept)>;
using TestFixture_void_function_void_noexcept =
    TestFixtureConcept1<void_function_void_noexcept>;

using BasicFunctionConcept1 =
    START_EDCPP_TEST_TYPE_COLLECTION void_function_void,
      void_function_void_noexcept,
      type_container<void, decltype(intptr_testfunction_ptr_noexcept)>,
      type_container<void, decltype(intptr_testfunction_ptr)>,
      type_container<void, decltype(void_testfunction_ptr)>
          END_EDCPP_TEST_TYPE_COLLECTION;
TYPED_TEST_SUITE(TestFixtureConcept1, BasicFunctionConcept1);
#ifdef TT01

/// @test validate some of the strategy behind developing our traits.
TVOID TYPED_TEST_TT01(TestFixtureConcept1, FunctionPtr) {
  TestFixture::printTypes();
  PRINT_FUNCTION_TYPE(TestFixture::F);
}
#endif
#ifdef UT01
/// @test validate tools for trait testing of simplest function pointer void()
TVOID TEST_UT01(Traits, F_void_function_void_noexcept) {
  using TestFixture = TestFixture_void_function_void_noexcept;
  PRINT_FUNCTION_TYPE(TestFixture::F);
  TEST_PROBE(P1010, EXPECT_FALSE(TestFixture::is_const));
  TEST_PROBE(P1020, EXPECT_FALSE(TestFixture::is_except));
  TEST_PROBE(P1030, EXPECT_TRUE(TestFixture::is_invocable));
  TEST_PROBE(P1040, EXPECT_TRUE(TestFixture::is_function));
  TEST_PROBE(P1050, EXPECT_TRUE(TestFixture::is_function_pointer));
  TEST_PROBE(P1060, EXPECT_FALSE(TestFixture::is_functor));
  TEST_PROBE(P1070, EXPECT_TRUE(TestFixture::is_invocable));
  TEST_PROBE(P1080, EXPECT_FALSE(TestFixture::is_lambda));
  TEST_PROBE(P1090, EXPECT_FALSE(TestFixture::is_member_function_pointer));
  TEST_PROBE(P1100, EXPECT_EQ(0, TestFixture::numargs));
  TEST_PROBE(P1110, EXPECT_TRUE(TestFixture::is_noexcept));
  TEST_PROBE(P1120, EXPECT_TRUE(TestFixture::is_return_void));
}
#endif
