#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "util/ghelper.h"
#include "senders/default_sender.h"
#include "util/unit_test.h"
#include "test_functions.h"
void void_f_p1_ptr_except(int* ctr) {
  *ctr += 1;
}
void void_f_p1_ptr_noexcept(int* ctr) noexcept {
  *ctr += 1;
}
void void_testfunction_ptr(int a, int* b) {
  *b += a;
}
int* intP_testfunction_ptr(int a, int* b) {
  *b += a;
  return b;
}
int* intP_testfunction_ptr_noexcept(int a, int* b) noexcept {
  *b += a;
  return b;
}
void void_f_except() {}                      //
void void_f_noexcept() noexcept {}           //
void void_f_int_except(int i) {}             //
void void_f_int_noexcept(int i) noexcept {}  //
void void_f_intR_except(int& i) {
  i++;
}  //
void void_f_intR_noexcept(int& i) noexcept {
  i++;
}  //
void void_f_intRR_except(int&& i) {
  i++;
}
void void_f_intRR_noexcept(int&& i) noexcept {
  i++;
}
void void_f_intRC_except(int const& i) {}
void void_f_intRC_noexcept(int const& i) noexcept {}
void void_f_intRRC_except(int const&& i) {}
void void_f_intRRC_noexcept(int const&& i) noexcept {}
void void_f_intP_except(int* l_int) {
  *l_int++;
}
void void_f_intPC_except(int const* l_int) {}
void void_f_intPC_noexcept(int const* l_int) noexcept {}
void void_f_intP_noexcept(int* l_int) noexcept {
  *l_int++;
}

int int_f_except() {
  return 5;
}  //
int int_f_noexcept() noexcept {
  return 5;
}  //
int int_f_int_except(int i) {
  return i;
}  //
int int_f_int_noexcept(int i) noexcept {
  return i;
}  //
int int_f_intR_except(int& i) {
  i++;
  return 5;
}  //
int int_f_intR_noexcept(int& i) noexcept {
  i++;
  return 5;
}  //
int int_f_intRR_except(int&& i) {
  i++;
  return 5;
}
int int_f_intRR_noexcept(int&& i) noexcept {
  i++;
  return 5;
}
int int_f_intRC_except(int const& i) {
  return 5;
}
int int_f_intRC_noexcept(int const& i) noexcept {
  return 5;
}
int int_f_intRRC_except(int const&& i) {
  return 5;
}
int int_f_intRRC_noexcept(int const&& i) noexcept {
  return 5;
}
int int_f_intP_except(int* l_int) {
  *l_int++;
  return 5;
}
int int_f_intPC_except(int const* l_int) {
  return 5;
}
int int_f_intPC_noexcept(int const* l_int) noexcept {
  return 5;
}
int int_f_intP_noexcept(int* l_int) noexcept {
  *l_int++;
  return 5;
}
class _TC1 {
 public:
  _TC1() {}
  _TC1(int i) {}
  _TC1(_TC1 const& tc) {}
  _TC1(_TC1&& tc) {}
  _TC1& operator=(_TC1 const& rs) { return *this; }
  _TC1& operator=(_TC1&& rs) { return *this; }
};
_TC1 tc;
_TC1& _TC1R_f_except() {
  return tc;
}
_TC1 _TC1_f_noexcept() noexcept {
  return tc;
}  //
_TC1 const& _TC1CR_f_int_except(int i, _TC1& tc1) {
  return tc1;
}  //
_TC1 _TC1_f_int__TC1_noexcept(int i, _TC1 tc1) noexcept {
  return tc1;
}  //
_TC1& _TC1R_f__TC1R_except(_TC1& i) {
  return i;
}  //
_TC1& _TC1R_f_intR__TC1RR_noexcept(int& i, _TC1&& t) noexcept {
  i++;
  return t;
}  //
// Default Delegate
void void_f_const_Info_noexcept(_EDCPP Info const& info) noexcept {}
T09998
T09999
T10000
T10001
