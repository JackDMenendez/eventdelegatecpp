#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H
#include "util/ghelper.h"
#include "util/unit_test.h"
EDCPP_UNIT_TEST_BEGIN
void void_f_except();
void void_f_noexcept() noexcept;

#define VOID_L_EXCEPT auto void_l_except = [] {}
#define VOID_L_NOEXCEPT auto void_l_noexcept = []() noexcept {}

void void_f_p1_ptr_except(int* ctr);
void void_f_p1_ptr_noexcept(int* ctr) noexcept;
void void_testfunction_ptr(int a, int* b);
int* intptr_testfunction_ptr(int a, int* b);
int* intptr_testfunction_ptr_noexcept(int a, int* b) noexcept;
EDCPP_UNIT_TEST_END
#endif
