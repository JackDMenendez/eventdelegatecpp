#ifndef GHELPER_H
#define GHELPER_H
#include <gtest/gtest.h>
/**
 * @brief A dummy return type to make TEST functions look like functions to the
 * documentation software.
 */
#define TVOID
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
#else
#define TEST_OUTPUT() std::cout
#define MINFO std::cout
#endif
// Use these to create TEST_P so tha Doxygen does not get confused
#define PARAMETERIZED_TEST_PUT1(FIXTURE) TEST_P(FIXTURE, PUT1)
#define PARAMETERIZED_TEST_PUT2(FIXTURE) TEST_P(FIXTURE, PUT2)
#define PARAMETERIZED_TEST_PUT3(FIXTURE) TEST_P(FIXTURE, PUT3)
/**
@brief A hack that allows printing of test information to GTest output
*/
#define TEST_INFO() TEST_OUTPUT() << "[     INFO ] "
#define TEST_TRACE() TEST_OUTPUT() << "[     TRCE ] "
#define MMSG(MI) MINFO << "[     " << MTOSTRING(MI) << " ] "
#define TEST_PROBE(PID, PASSERTION)          \
  MMSG(INFO) << MTOSTRING(PID) << std::endl; \
  PASSERTION
enum probeoperator { equal, not_equal };
template <typename T, typename D>
auto probe(const std::string id, const T lhs, const D rhs, probeoperator prbop)
    -> testing::AssertionResult {
  MMSG(INFO) << id << std::endl;
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
#define MISEQUAL(PID, PLHS, PRHS) \
  probe(MTOSTRING(PID), PLHS, PRHS, probeoperator::equal)
#define MISNOTEQUAL(PID, PLHS, PRHS) \
  probe(MTOSTRING(PID), PLHS, PRHS, probeoperator::not_equal)
#define TEST_UT1(testid, test_case_name) TEST(testid, test_case_name##T1)
#define TEST_UT2(testid, test_case_name) TEST(testid, test_case_name##UT2)
#define TEST_UT3(testid, test_case_name) TEST(testid, test_case_name##UT3)
#define TEST_UT4(testid, test_case_name) TEST(testid, test_case_name##UT4)
#define TEST_UT5(testid, test_case_name) TEST(testid, test_case_name##UT5)
#define TEST_UT6(testid, test_case_name) TEST(testid, test_case_name##UT6)
#define TEST_UT7(testid, test_case_name) TEST(testid, test_case_name##UT7)
#define TEST_UT8(testid, test_case_name) TEST(testid, test_case_name##UT8)
#define TEST_UT9(testid, test_case_name) TEST(testid, test_case_name##UT9)
#define TEST_UT10(testid, test_case_name) TEST(testid, test_case_name##UT10)

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
