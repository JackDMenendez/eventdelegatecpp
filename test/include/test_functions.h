#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H
#include "senders/default_sender.h"
#include "util/ghelper.h"
#include "util/unit_test.h"
EDCPP_UNIT_TEST_BEGIN
void void_f_except();
void void_f_noexcept() noexcept;

#define VOID_L_EXCEPT auto void_l_except = [] {}
#define VOID_L_NOEXCEPT auto void_l_noexcept = []() noexcept {}
void void_f_const_Info_noexcept(Info const&) noexcept;
void void_f_p1_ptr_except(int* ctr);
void void_f_p1_ptr_noexcept(int* ctr) noexcept;
void void_testfunction_ptr(int a, int* b);
int* intptr_testfunction_ptr(int a, int* b);
int* intptr_testfunction_ptr_noexcept(int a, int* b) noexcept;

using void_function_void = type_container<void, decltype(void_f_except)>;
using void_function_void_noexcept =
    type_container<void, decltype(void_f_noexcept)>;
using ED_UnitTest_TypeCollection =
    START_EDCPP_TEST_TYPE_COLLECTION void_function_void,
      void_function_void_noexcept,
      type_container<void, decltype(intptr_testfunction_ptr_noexcept)>,
      type_container<void, decltype(intptr_testfunction_ptr)>,
      type_container<void, decltype(void_testfunction_ptr)>,
      type_container<void, decltype(void_f_p1_ptr_except)>,
      type_container<void, decltype(void_f_p1_ptr_noexcept)>,
      type_container<void, decltype(void_f_const_Info_noexcept)>

          END_EDCPP_TEST_TYPE_COLLECTION;
EDCPP_UNIT_TEST_END
#endif
