#ifndef GHELPER_H
#define GHELPER_H
#include <gtest/gtest.h>
#include <iostream>
/**
 * @brief A dummy return type to make TEST functions look like functions to the
 * documentation software.
 */
#define TVOID
#define MTESTID
#define MTESTCASENAME
#ifndef MSTRINGIFY
#define MSTRINGIFY(x) #x
#define MTOSTRING(x) MSTRINGIFY(x)
#endif
#ifndef MCONCAT
#define MCONCAT(LHS, RHS) MCONCAT_(LHS, RHS)
#define MCONCAT_(LHS, RHS) LHS##RHS
#endif
#ifndef MNOOP
#define MNOOP(x) x
#endif
#ifdef _DEBUG
/**
@brief A macro used to determine whether output is emitted in gtest output
@details
When _DEBUG is defined then output will show up, otherwise it will not as
gtest hides std::cout but does not hide std::cerr.  The objective is to save
a lot of debugging time by emitting more information.
*/
#define TEST_OUTPUT() std::cerr
#define MINFO std::cerr
#define MTRACE std::cerr
#else
#define TEST_OUTPUT() std::cout
#define MINFO std::cout
#define MTRACE std::cout
#endif
#define UTDISABLED DISABLED_
// Use these to create TEST_P so tha Doxygen does not get confused
#define PARAMETERIZED_TEST_PUT1(FIXTURE) TEST_P(FIXTURE, PUT1)
#define PARAMETERIZED_TEST_PUT2(FIXTURE) TEST_P(FIXTURE, PUT2)
#define PARAMETERIZED_TEST_PUT3(FIXTURE) TEST_P(FIXTURE, PUT3)
#define TEST_EOL std::endl
/**
@brief A hack that allows printing of test information to GTest output
*/
#define TEST_INFO() TEST_OUTPUT() << "[     INFO ] "
#define TEST_TRACE() TEST_OUTPUT() << "[     TRCE ] "
#define MMSG(MI) MINFO << "[     " << MTOSTRING(MI) << " ] "
#define PRINT_FUNCTION_TYPE(mF)     \
  TEST_INFO() << "Function Type: "; \
  ArgPrinter<mF>::print();          \
  MINFO << TEST_EOL
#define TEST_PROBE(PID, PASSERTION)                                   \
  MMSG(INFO) << MTOSTRING(PID) << TEST_EOL; \
  PASSERTION
enum probeoperator { equal, not_equal };
template <typename T, typename D>
auto probe(const std::string id, const T lhs, const D rhs, probeoperator prbop)
    -> testing::AssertionResult {
  MMSG(INFO) << id << TEST_EOL;
  bool probe_outcome = false;
  std::string opr;
  switch (prbop) {
    case probeoperator::equal:
      probe_outcome = (lhs == rhs);
      opr = "==";
      break;
    case probeoperator::not_equal:
      probe_outcome = (lhs != rhs);
      opr = "!=";
      break;
  }
  return probe_outcome ? testing::AssertionSuccess()
                       : testing::AssertionFailure()
                             << " At Probe " << id << " " << lhs
                             << " == " << rhs;
};

template <typename T>
concept RVOID = _STD is_same_v<T, void>;
template <typename T>
concept RNOTVOID = !_STD is_same_v<T, void>;

template <class... T>
struct ArgPrinter;

template <>
struct ArgPrinter<void> {
  static void print() {}
};
template <class First>
struct ArgPrinter<First> {
  static void print() {
    MINFO << typeid(First).name();
    ArgPrinter<void>::print();
  }
};
template <class First, class... Rest>
struct ArgPrinter<First, Rest...> {
  static void print() {
    MINFO << typeid(First).name() << ", ";
    ArgPrinter<Rest...>::print();  // recursive call using pack expansion syntax
  }
};
/// @brief Contain to different types
///
/// @details Designed for use in parameterized typed tests that need two
/// template parameters
template <class A, class B>
struct type_container {
  using type_A = A;
  using type_B = B;
};

#define MISEQUAL(PID, PLHS, PRHS) \
  probe(MTOSTRING(PID), PLHS, PRHS, probeoperator::equal)
#define MISNOTEQUAL(PID, PLHS, PRHS) \
  probe(MTOSTRING(PID), PLHS, PRHS, probeoperator::not_equal)
#define TEST_UT01(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT01##test_case_name))
#define TEST_UT02(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT02##test_case_name))
#define TEST_UT03(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT03##test_case_name))
#define TEST_UT04(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT04##test_case_name))
#define TEST_UT05(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT05##test_case_name))
#define TEST_UT06(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT06##test_case_name))
#define TEST_UT07(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT07##test_case_name))
#define TEST_UT08(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT08##test_case_name))
#define TEST_UT09(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT09##test_case_name))
#define TEST_UT10(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT10##test_case_name))
#define TEST_UT11(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT11##test_case_name))
#define TEST_UT12(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT12##test_case_name))
#define TEST_UT13(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT13##test_case_name))
#define TEST_UT14(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT14##test_case_name))
#define TEST_UT15(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT15##test_case_name))
#define TEST_UT16(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT16##test_case_name))
#define TEST_UT17(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT17##test_case_name))
#define TEST_UT18(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT18##test_case_name))
#define TEST_UT19(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT19##test_case_name))
#define TEST_UT20(testid, test_case_name, ...) \
  TEST(testid, MCONCAT(__VA_ARGS__, UT20##test_case_name))

#define TYPED_TEST_TT01(testid, test_case_name) \
  TYPED_TEST(testid, TT01##test_case_name)
#define TYPED_TEST_TT02(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT02##test_case_name)
#define TYPED_TEST_TT03(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT03##test_case_name)
#define TYPED_TEST_TT04(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT04##test_case_name)
#define TYPED_TEST_TT05(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT05##test_case_name)
#define TYPED_TEST_TT06(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT06##test_case_name)
#define TYPED_TEST_TT07(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT07##test_case_name)
#define TYPED_TEST_TT08(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT08##test_case_name)
#define TYPED_TEST_TT09(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT09##test_case_name)
#define TYPED_TEST_TT10(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT10##test_case_name)
#define TYPED_TEST_TT11(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT11##test_case_name)
#define TYPED_TEST_TT12(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT12##test_case_name)
#define TYPED_TEST_TT13(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT13##test_case_name)
#define TYPED_TEST_TT14(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT14##test_case_name)
#define TYPED_TEST_TT15(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT15##test_case_name)
#define TYPED_TEST_TT16(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT16##test_case_name)
#define TYPED_TEST_TT17(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT17##test_case_name)
#define TYPED_TEST_TT18(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT18##test_case_name)
#define TYPED_TEST_TT19(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT19##test_case_name)
#define TYPED_TEST_TT20(testid, test_case_name, ...) \
  TYPED_TEST(testid, TT20##test_case_name)
#define NTEST_T1(test_case_name, test_name, ...) \
  NAMESPACED_TEST(test_case_name, MCONCAT(T1_, test_name), __VA_ARGS__)
#define NTEST_T2(test_case_name, test_name, ...) \
  NAMESPACED_TEST(test_case_name, MCONCAT(T2_, test_name), __VA_ARGS__)
#define NTEST_T3(test_case_name, test_name, ...) \
  NAMESPACED_TEST(test_case_name, MCONCAT(T3_, test_name), __VA_ARGS__)
#define NTEST_T4(test_case_name, test_name, ...) \
  NAMESPACED_TEST(test_case_name, MCONCAT(T4_, test_name), __VA_ARGS__)
#define NTEST_T5(test_case_name, test_name, ...) \
  NAMESPACED_TEST(test_case_name, MCONCAT(T5_, test_name), __VA_ARGS__)
#define NTEST_T6(test_case_name, test_name, ...) \
  NAMESPACED_TEST(test_case_name, MCONCAT(T6_, test_name), __VA_ARGS__)
#define NTEST_T7(test_case_name, test_name, ...) \
  NAMESPACED_TEST(test_case_name, MCONCAT(T7_, test_name), __VA_ARGS__)
#define NTEST_T8(test_case_name, test_name, ...) \
  NAMESPACED_TEST(test_case_name, MCONCAT(T8_, test_name), __VA_ARGS__)
#define NTEST_T9(test_case_name, test_name, ...) \
  NAMESPACED_TEST(test_case_name, MCONCAT(T9_, test_name), __VA_ARGS__)
#define NTEST_T10(test_case_name, test_name, ...) \
  NAMESPACED_TEST(test_case_name, MCONCAT(T10_, test_name), __VA_ARGS__)

#define TEST_F_UT1(FXTR) TEST_F(FXTR, UT1)
#define TEST_F_UT2(FXTR) TEST_F(FXTR, UT2)
#define TEST_F_UT3(FXTR) TEST_F(FXTR, UT3)
#define TEST_F_UT4(FXTR) TEST_F(FXTR, UT4)
#define TEST_F_UT5(FXTR) TEST_F(FXTR, UT5)
#define TEST_F_UT6(FXTR) TEST_F(FXTR, UT6)
#define TEST_F_UT7(FXTR) TEST_F(FXTR, UT7)
#define TEST_F_UT8(FXTR) TEST_F(FXTR, UT8)
#define TEST_F_UT9(FXTR) TEST_F(FXTR, UT9)

#define NTEST_UT1(test_fixture, test_name, ...) \
  NAMESPACED_TEST_F(test_fixture, MCONCAT(T1_, test_name), __VA_ARGS__)
#define NTEST_UT2(test_fixture, test_name, ...) \
  NAMESPACED_TEST_F(test_fixture, MCONCAT(T2_, test_name), __VA_ARGS__)
#define NTEST_UT3(test_fixture, test_name, ...) \
  NAMESPACED_TEST_F(test_fixture, MCONCAT(T3_, test_name), __VA_ARGS__)
#define NTEST_UT4(test_fixture, test_name, ...) \
  NAMESPACED_TEST_F(test_fixture, MCONCAT(T4_, test_name), __VA_ARGS__)
#define NTEST_UT5(test_fixture, test_name, ...) \
  NAMESPACED_TEST_F(test_fixture, MCONCAT(T5_, test_name), __VA_ARGS__)
#define NTEST_UT6(test_fixture, test_name, ...) \
  NAMESPACED_TEST_F(test_fixture, MCONCAT(T6_, test_name), __VA_ARGS__)
#define NTEST_UT7(test_fixture, test_name, ...) \
  NAMESPACED_TEST_F(test_fixture, MCONCAT(T7_, test_name), __VA_ARGS__)
#define NTEST_UT8(test_fixture, test_name, ...) \
  NAMESPACED_TEST_F(test_fixture, MCONCAT(T8_, test_name), __VA_ARGS__)
#define NTEST_UT9(test_fixture, test_name, ...) \
  NAMESPACED_TEST_F(test_fixture, MCONCAT(T9_, test_name), __VA_ARGS__)
#define NTEST_UT10(test_fixture, test_name, ...) \
  NAMESPACED_TEST_F(test_fixture, MCONCAT(T10_, test_name), __VA_ARGS__)
#endif
