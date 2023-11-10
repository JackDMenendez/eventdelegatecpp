#include "test_functions.h"
#include "util/unit_test.h"
#include "senders/default_sender.h"
void EDCPP_UNIT_TEST_NAMESPACE::void_f_p1_ptr_except(int* ctr) {
  *ctr += 1;
}
void EDCPP_UNIT_TEST_NAMESPACE::void_f_p1_ptr_noexcept(int* ctr) noexcept {
  *ctr += 1;
}
void EDCPP_UNIT_TEST_NAMESPACE::void_testfunction_ptr(int a, int* b) {
  *b += a;
}
int* EDCPP_UNIT_TEST_NAMESPACE::intptr_testfunction_ptr(int a, int* b) {
  *b += a;
  return b;
}
int* EDCPP_UNIT_TEST_NAMESPACE::intptr_testfunction_ptr_noexcept(
    int a,
    int* b) noexcept {
  *b += a;
  return b;
}
void EDCPP_UNIT_TEST_NAMESPACE::void_f_except() {}
void EDCPP_UNIT_TEST_NAMESPACE::void_f_noexcept() noexcept {}
void EDCPP_UNIT_TEST_NAMESPACE::void_f_const_Info_noexcept(Info const& info) noexcept {}
