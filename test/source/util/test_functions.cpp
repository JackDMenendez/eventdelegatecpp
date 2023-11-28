#include "test_functions.h"
#include "senders/default_sender.h"
#include "util/unit_test.h"
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
void void_f_int_except(int i) {}
void void_f_int_except(int const& i) {}
void void_f_intR_except(int& i) {
  i++;
}
void void_f_intRC_except(int const& i) {}
void EDCPP_UNIT_TEST_NAMESPACE::void_f_const_Info_noexcept(
    Info const& info) noexcept {}
