#include <gtest/gtest.h>
#include <functional>
#include <type_traits>
#define MTESTREGIME 1
#include <concepts>
#include "ed_traits.h"
#include "event_concepts.h"
#include "senders/default_sender.h"
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
  // second tier of traits
  static constexpr bool eligible_for_simple_delegate = is_return_void;
  static constexpr bool eligible_for_complex_delegate = true;
  static constexpr bool eligible_for_standard_delegate =
      false;  // todo fix this
  static constexpr bool eligible_for_default_delegate =
      (_EDCPP is_default_delegate_v<F>);
};
VOID_L_EXCEPT;
VOID_L_NOEXCEPT;

using TestFixture_void_function_void = TestFixtureConcept1<void_function_void>;
using TestFixture_void_function_void_noexcept =
    TestFixtureConcept1<void_function_void_noexcept>;

using TestFixture_void_f_const_info_noexcept =
    TestFixtureConcept1<decltype(void_f_const_Info_noexcept)>;
using BasicFunctionConcept1 = ED_UnitTest_TypeCollection;

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
/// 
/// We are using the values in TestFixture to test the functioning of func_y
TVOID TEST_UT01(Traits, F_void_function_void_noexcept) {
  using TestFixture = TestFixture_void_function_void_noexcept;
  PRINT_FUNCTION_TYPE(TestFixture::F);  // Function type shows up in the output.
                                        // This is a pointer type.
  // Direct Trait Test

  TEST_PROBE(P0110, EXPECT_FALSE(is_default_delegate_v<TestFixture::F>));
  using TestType = typename func_y<TestFixture::R,  // return code
                                   true,            // noexcept
                                   false,           // const
                                   false,           // reference
                                   false,           // rvalue reference
                                   void  // list of function Arguments
                                   >::FP;
  PRINT_FUNCTION_TYPE(TestType);  // very useful for debugging compare to the
                                  // first printed function type
  auto same_type = _STD is_same_v<TestFixture::F, TestType>;
  TEST_PROBE(P0111, EXPECT_TRUE(same_type));
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
  TEST_PROBE(P1130, EXPECT_TRUE(TestFixture::eligible_for_simple_delegate));
  TEST_PROBE(P1140, EXPECT_TRUE(TestFixture::eligible_for_complex_delegate));
  TEST_PROBE(P1150, EXPECT_FALSE(TestFixture::eligible_for_standard_delegate));
  TEST_PROBE(P1160, EXPECT_FALSE(TestFixture::eligible_for_default_delegate));
}
#endif
#ifdef UT02
/// @test Testing the DefaultDelegate´s traits.
TVOID TEST_UT02(Traits, F_void_f_const_Info_noexcept) {
  using TestFixture = TestFixture_void_f_const_info_noexcept;
  PRINT_FUNCTION_TYPE(TestFixture::F);
  // Type related Tests
  TEST_PROBE(P0110, EXPECT_TRUE(is_default_delegate_v<TestFixture::F>));
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
  TEST_PROBE(P1130, EXPECT_TRUE(TestFixture::eligible_for_simple_delegate));
  TEST_PROBE(P1140, EXPECT_TRUE(TestFixture::eligible_for_complex_delegate));
  TEST_PROBE(P1150, EXPECT_FALSE(TestFixture::eligible_for_standard_delegate));
  TEST_PROBE(P1160, EXPECT_FALSE(TestFixture::eligible_for_default_delegate));
}
#endif
