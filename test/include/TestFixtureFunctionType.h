#ifndef TESTFIXTURE_FUNCTIONTYPE_H
#define TESTFIXTURE_FUNCTIONTYPE_H
#include <gtest/gtest.h>
#include "ed_traits.h"
#include "util/ghelper.h"
#include "util/unit_test.h"
EDCPP_UNIT_TEST_BEGIN
template <bool iNXCPT,
          bool iCNST,
          size_t xPRMCNT,
          bool xDFLTDLGT,
          bool xPSTDDSTDDLGT>
class TestFunction_TypeConstants {
 public:
  static constexpr bool input_noexcept_t = iNXCPT;
  static constexpr bool input_const = iCNST;
  static constexpr size_t expected_parameter_count = xPRMCNT;
  static constexpr bool expect_default_delegate = xDFLTDLGT;
  static constexpr bool expect_standard_delegate = xPSTDDSTDDLGT;
  static constexpr bool expect_noexcept = input_noexcept_t;
  static constexpr bool expect_const = input_const;
};
/// This template class is used as a TestFixture´s TypeParam inside a Google
/// Typed Test.
///
/// To access this data from within the test body, the special name TypeParam
/// refers to this data, refers to the TestFixture refers to the fixture class.
/// https://google.github.io/googletest/reference/testing.html
///
/// Google Type Tests use a list of types to create a single test against a list
/// of types. Each type in the list instantiates a single test fixture class
/// template which has a body that tests the outputs of the test fixture against
/// the input type. Unfortunately a given test fixture can only accept one type,
/// whence we create this container of many parameters as a single type to pass
/// to the Google type testing facility. This class is used to provide a test
/// function type and data needed to validate our ability to parse a function
/// type. The first six template parameters are input data for a test and the
/// last four are expected results. The expected results are used to validate
/// the parsing of the function type.
///
/// This class is used to create test fixtures for Google Test, testing a list
/// of function types. Instantiations of this class appear in the Google Test
/// testing::types list used in a Google Typed Test. The goal is to facilitate a
/// large number of test cases that can be added via the TFFT macro (see below).
///
/// Input values specified by each instantiation of this template are prefixed
/// with the "input_". Expected values are prefixed with "expect_". This
/// convention exists only to make the purposes clear the coder. For example,
/// input_noexcept_t is the input value for whether noexcept in specified in the
/// input function. expect_noexcept is the expected output of the test. Both
/// values are initialized via the iNXCPT template parameter.
///
/// Regarding the template parameters, the parameters that begin with lower case
/// "i" are inputs to the test and the parameters that begin with lower case "x"
/// are expected outputs. The expected values of some inputs are not specified
/// when and if they are the same as the corresponding input. For example the
/// input parameter iNXCPT specifies the input that noexcept is or is not
/// specified by the function and the expected value is the same as the input
/// value.
///
/// @tparam iFNCTNPTR the type of the test input function pointer type for
/// example int(*)(int) const noexcept or decltype(&foo) where foo is a
/// function.
/// @tparam iRTRNCD the type of the test input return type such as int, void,
/// etc
/// @tparam iNXCPT boolean test input noexcept indicator true means noexcept was
/// specified false means noexcept was not specified. There is no corresponding
/// expected value for this parameter because this parameter is reflected by the
/// container as output.
/// @tparam iCNST boolean test input const indicator true means the const
/// keyword was specified, false means the const keyword was specified. There is
/// no corresponding expected value for this parameter because this parameter is
/// reflected by the container as output.
/// @tparam xPRMCNT sizeof test output expected parameter count, the number of
/// parameters expected to be in the function type specified by iFNCTNPTR.
/// @tparam xDFLTDLGT boolean test output expecting default delegate, true
///  means the iFNCNPTR is a default delegate, false means the function is not a
///  default delegate.
/// @tparam xPSTDDSTDDLGT boolean test output expecting a standard delegate
/// which is any function that has a parameter inheriting from Info. True means
/// that the function is a standard delegate and false means the function is not
/// a standard delegate.
/// @tparam xINPARAMS expected function parameter type list. These are the
/// parameters to the function defined by iFNCTNPTR.
template <class iFNCTNPTR,
          class iRTRNCD,
          bool iNXCPT,
          bool iCNST,
          size_t xPRMCNT,
          bool xDFLTDLGT,
          bool xPSTDDSTDDLGT,
          class... xINPARAMS>
class TestFunction_TypeContainer
    : public TestFunction_TypeConstants<iNXCPT,
                                        iCNST,
                                        xPRMCNT,
                                        xDFLTDLGT,
                                        xPSTDDSTDDLGT> {
 public:
  /// @brief The input function return type.
  using input_return_type_t = iRTRNCD;
  /// @brief The input function pointer type.
  using input_function_pointer_type_t = iFNCTNPTR;
};
template <class iFNCTNPTR,
          bool iNXCPT,
          bool iCNST,
          size_t xPRMCNT,
          bool xDFLTDLGT,
          bool xPSTDDSTDDLGT,
          class... xINPARAMS>
class TestFunction_TypeContainer<iFNCTNPTR,
                                 TheVoidType,
                                 iNXCPT,
                                 iCNST,
                                 xPRMCNT,
                                 xDFLTDLGT,
                                 xPSTDDSTDDLGT,
                                 xINPARAMS...>
    : public TestFunction_TypeConstants<iNXCPT,
                                        iCNST,
                                        xPRMCNT,
                                        xDFLTDLGT,
                                        xPSTDDSTDDLGT> {
 public:
  /// @brief The input function return type.
  using input_return_type_t = TheVoidType;
  /// @brief The input function pointer type.
  using input_function_pointer_type_t = iFNCTNPTR;
};
/// General case of the test fixture for testing function types.
template <class T>
class TestFixture_FunctionType : public testing::Test {};
/// This is accessed by the TestFixture type in a Google Typed Test designed.
///
/// Within the test body, the special name TypeParam refers to the type
/// parameter, and TestFixture refers to the fixture class.
/// https://google.github.io/googletest/reference/testing.html
///
template <class FNCTNPTR,
          class RTRNCD,
          bool NXCPT,
          bool CNST,
          size_t XPRMCNT,
          bool XDFLTDLGT,
          bool XPSTDDSTDDLGT,
          class... PARAMS>
class TestFixture_FunctionType<TestFunction_TypeContainer<FNCTNPTR,
                                                          RTRNCD,
                                                          NXCPT,
                                                          CNST,
                                                          XPRMCNT,
                                                          XDFLTDLGT,
                                                          XPSTDDSTDDLGT,
                                                          PARAMS...>>
    : public testing::Test,
      public _EDCPP func_yield_traits<RTRNCD, NXCPT, CNST, PARAMS...> {
 public:
  static void printTypes() {
    TEST_TRACE() << "Args";
    ArgPrinter<PARAMS...>::print();
    MINFO << ")" << TEST_EOL;
  }
};
#define TFFT(F, R, N, C, XPRM, XDFLT, XSTDDFLT, ...)                       \
  TestFunction_TypeContainer<decltype(&F), R, N, C, XPRM, XDFLT, XSTDDFLT, \
                             __VA_ARGS__>
EDCPP_UNIT_TEST_END
#endif
