/*
   EventDelegateCPP is a code library that provide event delegate functionality
   to C++

    Copyright (C) 2023  Jack D. Menendez

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
/// @file ConceptsUnitGTest.cpp
///
/// @brief Contains traits and concepts unit tests.
///
/// @ingroup EventDelegatCPP
///
/// @details
/// This is a Google Test file.The following is a list of the unit tests in this
/// file.
///
///      Test ID     | Test Description
/// ---------------- | ----------------
/// @ref A_TT01 TT01 | validate the func_yield_traits with function pointers.
///
/// @copyright 2023,2024 Jack D Menendez
/// @author Jack D. Menendez
/// @date date_here Created
/// @since @22/11//2023 Created
/// @version 0.0.1
#include <gtest/gtest.h>
#include <functional>
#include <type_traits>
#include <vector>
#define MTESTREGIME 1
#include <concepts>
#include "TestFixtureFunctionType.h"
#include "ed_traits.h"
#include "event_concepts.h"
#include "senders/default_sender.h"
#include "test_functions.h"
#include "util/ghelper.h"
#include "util/unit_test.h"

// uncommenting the following lines enables the unit tests
#define TT01 TRAITS_TT_01
EDCPP_USING
USING_EDCPP_UNIT_TEST

DECLARE_TEST_TYPE_COLLECTION(ED_UnitTest_TypeCollection,
                             TFFT(void_f_except,
                                  TheVoidType,
                                  TheVoidType,
                                  false,
                                  false,
                                  0,
                                  false,
                                  false,
                                  TheVoidType),
                             TFFT(void_f_noexcept,
                                  TheVoidType,
                                  TheVoidType,
                                  true,
                                  false,
                                  0,
                                  false,
                                  false,
                                  TheVoidType),
                             TFFT(void_f_int_except,
                                  TheVoidType,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int),
                             TFFT(void_f_int_noexcept,
                                  TheVoidType,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int),
                             TFFT(void_f_intR_except,
                                  TheVoidType,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int&),
                             TFFT(void_f_intR_noexcept,
                                  TheVoidType,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int&),
                             TFFT(void_f_intRR_except,
                                  TheVoidType,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int&&),
                             TFFT(void_f_intRR_noexcept,
                                  TheVoidType,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int&&),
                             TFFT(void_f_intRC_except,
                                  TheVoidType,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const&),
                             TFFT(void_f_intRC_noexcept,
                                  TheVoidType,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const&),
                             TFFT(void_f_intRRC_except,
                                  TheVoidType,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const&&),
                             TFFT(void_f_intRRC_noexcept,
                                  TheVoidType,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const&&),
                             TFFT(void_f_intP_except,
                                  TheVoidType,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int*),
                             TFFT(void_f_intPC_except,
                                  TheVoidType,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const*),
                             TFFT(void_f_intPC_noexcept,
                                  TheVoidType,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const*),
                             TFFT(void_f_intP_noexcept,
                                  TheVoidType,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int*),
                             TFFT(int_f_except,
                                  int,
                                  TheVoidType,
                                  false,
                                  false,
                                  0,
                                  false,
                                  false,
                                  TheVoidType),
                             TFFT(int_f_noexcept,
                                  int,
                                  TheVoidType,
                                  true,
                                  false,
                                  0,
                                  false,
                                  false,
                                  TheVoidType),
                             TFFT(int_f_int_except,
                                  int,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int),
                             TFFT(int_f_int_noexcept,
                                  int,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int),
                             TFFT(int_f_intR_except,
                                  int,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int&),
                             TFFT(int_f_intR_noexcept,
                                  int,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int&),
                             TFFT(int_f_intRR_except,
                                  int,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int&&),
                             TFFT(int_f_intRR_noexcept,
                                  int,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int&&),
                             TFFT(int_f_intRC_except,
                                  int,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const&),
                             TFFT(int_f_intRC_noexcept,
                                  int,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const&),
                             TFFT(int_f_intRRC_except,
                                  int,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const&&),
                             TFFT(int_f_intRRC_noexcept,
                                  int,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const&&),
                             TFFT(int_f_intP_except,
                                  int,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int*),
                             TFFT(int_f_intPC_except,
                                  int,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const*),
                             TFFT(int_f_intPC_noexcept,
                                  int,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int const*),
                             TFFT(int_f_intP_noexcept,
                                  int,
                                  TheVoidType,
                                  true,
                                  false,
                                  1,
                                  false,
                                  false,
                                  int*),
                             ///////////////////
                             TFFT(_TC1R_f_except,  //
                                  _TC1&,
                                  TheVoidType,
                                  false,
                                  false,
                                  0,
                                  false,
                                  false,
                                  TheVoidType),
                             TFFT(_TC1_f_noexcept,
                                  _TC1,
                                  TheVoidType,
                                  true,
                                  false,
                                  0,
                                  false,
                                  false,
                                  TheVoidType),
                             TFFT(_TC1_f_int__TC1_noexcept,
                                  _TC1,
                                  TheVoidType,
                                  true,
                                  false,
                                  2,
                                  false,
                                  false,
                                  int,
                                  _TC1),
                             TFFT(_TC1R_f__TC1R_except,
                                  _TC1&,
                                  TheVoidType,
                                  false,
                                  false,
                                  1,
                                  false,
                                  false,
                                  _TC1&),
                             TFFT(_TC1R_f_intR__TC1RR_noexcept,
                                  _TC1&,
                                  TheVoidType,
                                  true,
                                  false,
                                  2,
                                  false,
                                  false,
                                  int&,
                                  _TC1&&),
                             C09998,
                             C09999,
                             C10000,
                             C10001,
                             C10002);  // ED_UnitTest_TypeCollection

/// Create a test suite named TestFixture_FunctionType
///
/// Create the test suite from the TestFixture called TestFixture_FunctionType
/// and the TypeCollection called ED_UnitTest_TypeCollection
TYPED_TEST_SUITE(TestFixture_FunctionType, ED_UnitTest_TypeCollection);
#ifdef TT01

/// @anchor A_TT01
///
/// @brief TYPED_TEST_TT01 RegularFunctionPtrs, validate the func_yield_traits
///
/// @test TYPED_TEST_TT01 RegularFunctionPtrs, validate the func_yield_traits
/// class against a long list of regular function pointers that do not include
/// non-static class methods and functors or lambdas.
///
/// @details
/// TestFixture_FunctionType inherits from test::Test and from func_yield_traits
/// making all of the traits used by the Delegate class. This test validates
/// that all the traits are parsed properly.
///
/// Test Objectives
/// ---------------
/// Validate that different function pointer types that have equivalent function
/// types yield the same function type. This allows all equivalent functions to
/// instantiate the same Delegate type and subscribe to the same event. For
/// example, the function pointer types:
/// <code>
/// TypeParam::input_function_pointer_type_t(int __cdecl(int * __ptr64)
/// noexcept) TestFixture::FunctionPointer_t(int(__cdecl*)(int* __ptr64)
/// noexcept)
/// </code>
/// Are equivalent functions as far as the delegate is concerned.
///
/// Note that the two function pointer types above are equivalent but not the
/// same type.
/// Please see @ref function_equivalence Function Equivalence for more.
///
/// The following chart shows the function types supported by this
/// specialization of func_yield_traits.h in ed_traits. The test functions can b
/// found in Please keep the chart up
/// to date.
///
///         Function Type         | Test Function(s) test_functions.h
/// ----------------------------- | ---------------------------------
/// R(Args ...) noexcept const && |
/// R() noexcept const &&         |
/// R(Args ...) noexcept const &  |
///
/// The following tests are performed for each of the test functions type in the
/// ED_UnitTest_TypeCollection. Each probe listed has its result written to the
/// Google Test output for each type of test function in the collection.
///
/// Within the test body, the special name TypeParam refers to the type
/// parameter, and TestFixture refers to the fixture class.
/// https://google.github.io/googletest/reference/testing.html
///
/// TYPED_TEST_TT01 is a custom macro that gives the test body a unique name
/// that looks like a function to Doxygen allowing it to be documented as a
/// function.
///
/// TEST_PROBEW is a custom macro that writes the result of the probe to the
/// Google Test output. Each probe is given a unique probe number such that it
/// can be easily referenced and found in the output and code.
///
/// Probe | Description
/// ----- | -----------
/// P1000 | Test ability to parse the parameters of the function type.
/// P1020 | Test detection of default delegate.
/// P1030 | Test detection of standard delegate.
/// P1050 | Test reflection of the return type.
/// P1060 | Test reflection of the noexcept.
///
/// @return TVOID is a fake type that is used to fool Doxygen into documenting
/// the test as a function.
TVOID TYPED_TEST_TT01(TestFixture_FunctionType) {
  TEST_TRACE() << "TYPED_TEST_TT01 RegularFunctionPtrs typed test" << TEST_EOL;
  ////////////////////////////////////////////////////////////////////////
  TEST_TRACE() << "Test Function Equivalence" << TEST_EOL;
  TEST_TRACE() << "func_yield_traits specialization: " << TestFixture::specialization_id_v << TEST_EOL;
  TEST_TRACE() << "TypeParam::input_function_pointer_type_t("
               << typeid(TypeParam::input_function_pointer_type_t).name() << ")"
               << TEST_EOL;
  TEST_TRACE() << "TestFixture::FunctionPointer_t("
               << typeid(TestFixture::FunctionPointer_t).name() << ")"
               << TEST_EOL;
  ////////////////////////////////////////////////////////////////////////
  TEST_PROBEW(P1000,
              EXPECT_EQ(TestFixture::function_param_count_v,
                        TypeParam::expected_parameter_count),
              << "TestFixture::function_param_count_v("
              << TestFixture::function_param_count_v << ") == "
              << "TypeParam::expected_param_count("
              << TypeParam::expected_parameter_count << ")");
  ////////////////////////////////////////////////////////////////////////
  TEST_PROBEW(P1020,
              EXPECT_EQ(TestFixture::is_default_delegate_v,
                        TypeParam::expect_default_delegate),
              << "TestFixture::is_default_delegate_v("
              << TestFixture::is_default_delegate_v << ") == "
              << "TypeParam::expect_default_delegate("
              << TypeParam::expect_default_delegate << ")");
  ////////////////////////////////////////////////////////////////////////
  TEST_PROBEW(P1030,
              EXPECT_EQ(TestFixture::is_standard_delegate_v,
                        TypeParam::expect_standard_delegate),
              << "TestFixture::is_standard_delegate_v("
              << TestFixture::is_standard_delegate_v << ") == "
              << "TypeParam::expect_standard_delegate("
              << TypeParam::expect_standard_delegate << ")");
  ////////////////////////////////////////////////////////////////////////
  TEST_TRACE() << "P1050 TypeParam::input_return_type_t("
               << typeid(TypeParam::input_return_type_t).name() << ")"
               << TEST_EOL;
  TEST_TRACE() << "P1050 TestFixture::ReturnCode_t("
               << typeid(TestFixture::ReturnCode_t).name() << ")" << TEST_EOL;
  auto l_expect_same_return_type =
      _STD is_same_v<TypeParam::input_return_type_t, TestFixture::ReturnCode_t>;
  TEST_PROBEW(P1050, EXPECT_TRUE(l_expect_same_return_type),
              << "ReturnCode_t matches input_return_type_t");
  ////////////////////////////////////////////////////////////////////////
  TEST_PROBEW(P1060,
              MTEST_EQ(TestFixture::noexcept_v, TypeParam::expect_noexcept),
              "Exception environment correct");
  //<< "TestType::noexcept_v(" << TestFixture::noexcept_v << ") == "
  //<< "TypeParam::expect_noexcept(" << TypeParam::expect_noexcept << ")");
  ////////////////////////////////////////////////////////////////////////
  TEST_PROBEW(P1070, EXPECT_EQ(TestFixture::const_v, TypeParam::expect_const),
              << "TestType::const_v is " << TypeParam::expect_const);
}
#endif
#ifdef UT01
/// @test Validate func_yield_traits against void_function_void_noexcept
TVOID TEST_UT01(Traits, F_void_function_void_noexcept) {
  using input_return_type_t = typename void_function_void_noexcept::type_A;
  using input_function_type = typename void_function_void_noexcept::type_B;
  constexpr bool input_noexcept_t = true;
  constexpr bool input_const = false;
  constexpr bool input_reference = false;
  constexpr bool input_rvalue_reference = false;
#define input_arguments void

  using TestType =
      typename func_yield_traits<input_return_type_t,     // return code type
                                 input_noexcept_t,        // exception regime
                                 input_const,             // const definition
                                 input_reference,         // reference
                                 input_rvalue_reference,  // rvalue reference
                                 input_arguments  // list of function Arguments
                                 >;

#undef input_arguments

  size_t const expected_param_count = 0;
  bool const expect_default_delegate = false;
  bool const expect_standard_delegate = false;
  bool const expect_noexcept = true;
  bool const expect_const = false;
  bool const expect_reference = false;
  bool const expect_rvalue_reference = false;

  TEST_PROBEW(
      P1000, EXPECT_EQ(TestType::function_param_count_v, expected_param_count),
      << "TestType::function_param_count_v == " << expected_param_count);

  TEST_PROBEW(
      P1020,
      EXPECT_EQ(TestType::is_default_delegate_v, expect_default_delegate),
      << "TestType::is_default_delegate_v == " << expect_default_delegate);

  TEST_PROBEW(
      P1030,
      EXPECT_EQ(TestType::is_standard_delegate_v, expect_standard_delegate),
      << "TestType::is_standard_delegate_v == " << expect_standard_delegate);

  TEST_TRACE() << "P1040 Expected Func Type("
               << typeid(input_function_type).name() << ")" << TEST_EOL;
  TEST_TRACE() << "P1040 Actual Func Type("
               << typeid(TestType::FunctionType_t).name() << ")" << TEST_EOL;

  auto expect_same_function_type =
      _STD is_same_v<input_function_type, TestType::FunctionType_t>;

  TEST_PROBEW(P1040, EXPECT_TRUE(expect_same_function_type),
              << "TestFunctionType matches input_function_type");

  TEST_TRACE() << "P1050 input_return_type_t("
               << typeid(input_return_type_t).name() << ")" << TEST_EOL;
  TEST_TRACE() << "P1050 Actual Return Type("
               << typeid(TestType::ReturnCode_t).name() << ")" << TEST_EOL;

  auto expect_same_return_type =
      _STD is_same_v<input_return_type_t, TestType::ReturnCode_t>;
  TEST_PROBEW(P1050, EXPECT_TRUE(expect_same_return_type),
              << "TestType::ReturnCode_t matches input_return_type_t");

  TEST_PROBEW(P1060, EXPECT_EQ(TestType::noexcept_v, expect_noexcept),
              << "TestType::noexcept_v is " << expect_noexcept);

  TEST_PROBEW(P1070, EXPECT_EQ(TestType::const_v, expect_const),
              << "TestType::const_v is " << expect_const);

  TEST_PROBEW(P1080, EXPECT_EQ(TestType::reference_v, expect_reference),
              << "TestType::reference_v is " << expect_reference);

  TEST_PROBEW(P1090,
              EXPECT_EQ(TestType::rvalue_reference_v, expect_rvalue_reference),
              << "TestType::rvalue_reference_v is " << expect_rvalue_reference);
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
