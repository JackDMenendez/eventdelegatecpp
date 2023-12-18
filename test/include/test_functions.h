#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "TestFixtureFunctionType.h"
#include "senders/default_sender.h"
#include "util/ghelper.h"
#include "util/unit_test.h"
EDCPP_UNIT_TEST_BEGIN
#define NOEXCEPT true
#define EXCEPT false
#define CONST true
#define NONCONST false
#define DEFAULT true
#define NONDEFAULT false
#define STANDARD true
#define NONSTANDARD false
#define TFFT(F, R, O, N, C, XPRM, XDFLT, XSTDDFLT, ...)                      \
  TestFunction_TypeContainer<decltype(F), R, O, N, C, XPRM, XDFLT, XSTDDFLT, \
                             __VA_ARGS__>
#define TCONT(TYPENUM, CONTAINER) using TYPENUM = CONTAINER;
template <class A, class B>
struct function_definition_container {
  using type_A = A;
  using type_B = B;
};
// Types for type testing
void void_f_except();                    //
void void_f_noexcept() noexcept;         //
void void_f_int_except(int);             //
void void_f_int_noexcept(int) noexcept;  //
void void_f_intR_except(int& i);
void void_f_intR_noexcept(int& i) noexcept;
void void_f_intRR_except(int&& i);
void void_f_intRR_noexcept(int&& i) noexcept;
void void_f_intRC_except(int const& i);
void void_f_intRC_noexcept(int const& i) noexcept;
void void_f_intRRC_except(int const&& i);
void void_f_intRRC_noexcept(int const&& i) noexcept;
void void_f_intP_except(int*);
void void_f_intPC_except(int const*);
void void_f_intPC_noexcept(int const*) noexcept;
void void_f_intP_noexcept(int*) noexcept;

int int_f_except();                    //
int int_f_noexcept() noexcept;         //
int int_f_int_except(int);             //
int int_f_int_noexcept(int) noexcept;  //
int int_f_intR_except(int& i);
int int_f_intR_noexcept(int& i) noexcept;
int int_f_intRR_except(int&& i);
int int_f_intRR_noexcept(int&& i) noexcept;
int int_f_intRC_except(int const& i);
int int_f_intRC_noexcept(int const& i) noexcept;
int int_f_intRRC_except(int const&& i);
int int_f_intRRC_noexcept(int const&& i) noexcept;
int int_f_intP_except(int*);
int int_f_intPC_except(int const*);
int int_f_intPC_noexcept(int const*) noexcept;
int int_f_intP_noexcept(int*) noexcept;
class _TC1 {};
_TC1& _TC1R_f_except();  //
_TC1 _TC1_f_noexcept() noexcept;
_TC1& const _TC1CR_f_int_except(int, _TC1&);
_TC1 _TC1_f_int__TC1_noexcept(int, _TC1) noexcept;
_TC1& _TC1R_f__TC1R_except(_TC1& i);
_TC1& _TC1R_f_intR__TC1RR_noexcept(int& i, _TC1&&) noexcept;
////////////////////////////////////////////////////////////////////////////////
// return code
#define R09998 int const*
// function name
#define F09998 f_09998
// The object type
#define O09998 TheVoidType
// function parameter(s)
#define A09998T1 int const* const
#define A09998D1 A09998T1 l_intCPC
// function body
#define B09998                                              \
  {                                                         \
    TEST_TRACE() << "f_09998 delegate running" << TEST_EOL; \
    return l_intCPC;                                        \
  }
// function definition
#define D09998 R09998 F09998(A09998D1) noexcept
// function implementation
#define T09998 D09998 B09998
/// @test function f_09998 container(s) C09998
/// function returns R09998 arguments A09998 with except
D09998;
// function type container for unit type testing
TCONT(C09998,
      TFFT(F09998,
           R09998,
           O09998,
           EXCEPT,
           NONCONST,
           1,
           NONDEFAULT,
           NONSTANDARD,
           A09998T1));
////////////////////////////////////////////////////////////////////////////////
// return code
#define R09999 int const* const
// function name
#define F09999 f_09999
// The object type
#define O09999 TheVoidType
// function parameter(s)
#define A0999T1 int const* const
#define A0999D1 A0999T1 l_intCPC
// function body
#define B09999                                              \
  {                                                         \
    TEST_TRACE() << "f_09999 delegate running" << TEST_EOL; \
    return l_intCPC;                                        \
  }
// function definition
#define D09999 R09999 F09999(A0999D1) noexcept
// function implementation
#define T09999 D09999 B09999
/// @test function f_09999 container(s) C09999
/// function returns R09999 arguments A09999 with noexcept
D09999;
// function type container for unit type testing
TCONT(C09999,
      TFFT(F09999,
           R09999,
           O09999,
           NOEXCEPT,
           NONCONST,
           1,
           NONDEFAULT,
           NONSTANDARD,
           A0999T1));
////////////////////////////////////////////////////////////////////////////////
// return code
#define R10000 int
// function name
#define F10000 f_10000
// The object type
#define O10000 TheVoidType
// function parameter(s)
#define A10000T1 int*
#define A10000D1 A10000T1 l_intP
// function body
#define B10000                                              \
  {                                                         \
    TEST_TRACE() << "f_10000 delegate running" << TEST_EOL; \
    return *l_intP;                                         \
  }
// function definition
#define D10000 R10000 F10000(A10000D1) noexcept
// function implementation
#define T10000 D10000 B10000
/// @test function f_10000 container(s) C10000
/// function returns R10000 arguments A10000
D10000;
// function type container for unit type testing
TCONT(C10000,
      TFFT(F10000,
           R10000,
           O10000,
           NOEXCEPT,
           NONCONST,
           1,
           NONDEFAULT,
           NONSTANDARD,
           A10000T1));
// auto void_l_except = []() {};
// auto void_l_noexcept = []() noexcept {};
////////////////////////////////////////////////////////////////////////////////
// return code
#define R10001 void
// function name
#define F10001 f_10001
// The object type
#define O10001 TheVoidType
// function parameter(s)
#define A10001T1 _EDCPP Info const&
#define A10001D1 A10001T1 l_Info
// function body
#define B10001 \
  { TEST_TRACE() << "f_10001 default delegate running" << TEST_EOL; }
// function definition
#define D10001 R10001 F10001(A10001D1) noexcept
// function implementation
#define T10001 D10001 B10001
/// @test function f_10001 container(s) C10001
/// function returns R10001 arguments A10001
D10001;
// function type container for unit type testing, TheVoidType is a type that is
// used for specialization for void return types.
TCONT(C10001,
      TFFT(F10001,
           TheVoidType,
           O10001,
           NOEXCEPT,
           NONCONST,
           1,
           DEFAULT,
           STANDARD,
           A10001T1));
////////////////////////////////////////////////////////////////////////////////
// return code
#define R10002 void
// function name
#define F10002 f_10002
// The object type
#define O10002 TheVoidType
// function parameter(s)
#define A10002T1 int*
#define A10002D1 A10002T1 l_intP
// function body
#define B10002                                     \
  {                                                \
    TEST_TRACE() << "f_10002 running" << TEST_EOL; \
    *l_intP += 1;                                  \
  }
// function definition
#define D10002 R10002 F10002(A10002D1) noexcept
// function implementation
#define T10002 D10002 B10002
/// @test function f_10002 container(s) C10000
/// function returns R10002 arguments A10002
/// increments value pointed to by l_intP
D10002;
// function type container for unit type testing, TheVoidType is a type that is
// used for specialization for void return types.
TCONT(C10002,
      TFFT(F10002,
           TheVoidType,
           O10002,
           NOEXCEPT,
           NONCONST,
           1,
           NONDEFAULT,
           NONSTANDARD,
           A10002T1));
void void_testfunction_intP(int a, int* b);
int* intP_testfunction_intP(int a, int* b);
int* intP_testfunction_intP_noexcept(int a, int* b) noexcept;
EDCPP_UNIT_TEST_END
#endif
