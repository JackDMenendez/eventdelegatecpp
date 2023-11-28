#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H
#include "TestFixtureFunctionType.h"
#include "senders/default_sender.h"
#include "util/ghelper.h"
#include "util/unit_test.h"
EDCPP_UNIT_TEST_BEGIN
template <class A, class B>
struct function_definition_container {
  using type_A = A;
  using type_B = B;
};
void void_f_except();
void void_f_noexcept() noexcept;
void void_f_int_except(int);
void void_f_intR_except(int& i);
void void_f_intRC_except(int const & i);

#define VOID_L_EXCEPT auto void_l_except = [] {}
#define VOID_L_NOEXCEPT auto void_l_noexcept = []() noexcept {}
void void_f_const_Info_noexcept(Info const&) noexcept;
void void_f_p1_ptr_except(int* ctr);
void void_f_p1_ptr_noexcept(int* ctr) noexcept;
void void_testfunction_ptr(int a, int* b);
int* intptr_testfunction_ptr(int a, int* b);
int* intptr_testfunction_ptr_noexcept(int a, int* b) noexcept;

///
DECLARE_TEST_TYPE_COLLECTION(ED_UnitTest_TypeCollection)
TFFT(void_f_int_except, TheVoidType, TheVoidType, false, false, 1, false, false, int),
    TFFT(void_f_intR_except, TheVoidType, TheVoidType, false, false, 1, false, false, int&),
    TFFT(void_f_intRC_except, TheVoidType, TheVoidType, false, false, 1, false, false, int const &),
    TFFT(void_f_except,
         TheVoidType, TheVoidType,
         false,
         false,
         0,
         false,
         false,
         TheVoidType),
    TFFT(void_f_noexcept,
         TheVoidType, TheVoidType,
         true,
         false,
         0,
         false,
         false,
         TheVoidType) END_TEST_TYPE_COLLECTION;  // ED_UnitTest_TypeCollection

/// Create a test suite named TestFixture_FunctionType
///
/// Create the test suite from the TestFixture called TestFixture_FunctionType
/// and the TypeCollection called ED_UnitTest_TypeCollection
TYPED_TEST_SUITE(TestFixture_FunctionType, ED_UnitTest_TypeCollection);
EDCPP_UNIT_TEST_END
#endif
