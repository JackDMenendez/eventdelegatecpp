#include <gtest/gtest.h>
#define MTESTREGIME 1
// The following tests are built, comment out to disable
// #define TT01 1
#define UT01 1
// #define UT02 2
// #define UT03 3
// #define UT04 4
// #define UT05 5
// #define UT06 6
// #define UT07 7
// #define UT08 8
// #define UT09 9
// #define UT10 10
// #define UT11 11
// #define UT12 12
// #define UT13 13
// #define UT14 14

#include "Delegate.h"
#include "test_functions.h"
#include "util/ghelper.h"
#include "util/unit_test.h"
using namespace EDCPP_COMBINE(unit_test);
EDCPP_USING
// TODO test for void return requirement
namespace EDCPP_COMBINE(tests) {
void test_base_subscription_scenarios();
}

void basicTestFunction(int* ctr) {
  *ctr += 1;
}
using BasicDelegateFunction = void(int*);  // void __cdecl(int * __ptr64)
/**
 * For testing with noexcept
 *
 * The noexcept keyword does not change a function type but we test for it
 * anyway so we know if something changes.
 */
void BasicDelegateFunctionNoexcept(int* ctr) noexcept {
  *ctr += 2;
}
void h(int* ctr) {
  *ctr += 3;
}
struct TestFixture1 {
  int executeCount = 0;
  bool delegateFired = false;
  explicit TestFixture1(int id) : executeCount(id) {}
  void increment(int* ctr) {
    executeCount++;
    *ctr += 1;
    if (executeCount == 10) {
      onCount10();
    }
  }
  void add2(int* ctr) const { *ctr += 2; }
  void Count10Reached() {
    delegateFired = true;
    MMSG("delegate fired");
  }
  virtual void onCount10() {}
};
#ifdef TT01
template <typename T>
class DelegateFixture : public testing::Test {
 public:
  using CounterDelegate = Delegate<T>;
};

using BasicFunctionTypes = testing::Types<BasicDelegateFunction>;
TYPED_TEST_SUITE(DelegateFixture, BasicFunctionTypes);

TVOID TYPED_TEST_TT01(DelegateFixture, BasicFuntions) {
  // To refer to typedefs in the fixture, add the 'typename TestFixture::'
  // prefix.  The 'typename' is required to satisfy the compiler.

  int test_counter = 0;
  int expected_count = 1;

  using TestBasicFunctionsType = typename TestFixture::CounterDelegate;
  TestBasicFunctionsType testDelegate(basicTestFunction);
  TestBasicFunctionsType testNoexceptDelegate(BasicDelegateFunctionNoexcept);
  testDelegate(&test_counter);
  TEST_PROBE(P1000, EXPECT_EQ(test_counter, expected_count));
  testNoexceptDelegate(&test_counter);
  expected_count += 2;
  TEST_PROBE(P1010, EXPECT_EQ(test_counter, expected_count));
}
#endif  // TT01
#ifdef UT01
/**
 * @test DelegateUnitGTest_UT1Delegate_Test.
 */
TVOID TEST_UT01(DelegateUnitGTest, Delegate) {
  using CounterDelegate = Delegate<void(int*)>;
  int test_counter = 0;
  const int expected_count = 1;
  CounterDelegate delegate(&void_f_p1_ptr_except);
  delegate(&test_counter);
  TEST_PROBE(P1000, EXPECT_EQ(test_counter, expected_count));
  // long use_count = delegate.get_ptr().use_count();
  // long expected_use_count = 1;
  // TEST_PROBE(P1010, EXPECT_EQ(use_count, expected_use_count));
  // bool has_stored_pointer = (delegate.get_ptr().get() != nullptr);
  // TEST_PROBE(P1020, EXPECT_TRUE(has_stored_pointer));
}
#endif
#ifdef UT02
TVOID TEST_UT02(DelegateUnitGTest, MemberDelegate) {
  const int fixtureInstance_initialcount = 0;
  const int fixtureInstance_expectedcount = 1;
  const int expected_testcount = 1;
  int testCount = 0;
  TestFixture1 fixtureInstance(fixtureInstance_initialcount);
  using CounterDelegate = Delegate<void(int*)>;
  CounterDelegate delegate(fixtureInstance, &TestFixture1::increment);
  delegate(&testCount);
  TEST_PROBE(P1000, EXPECT_EQ(testCount, expected_testcount));
  TEST_PROBE(P1010, EXPECT_EQ(fixtureInstance.executeCount,
                              fixtureInstance_expectedcount));
  long use_count = delegate.get_ptr().use_count();
  long expected_use_count = 1;
  TEST_PROBE(P1020, EXPECT_EQ(use_count, expected_use_count));
  bool has_stored_pointer = (delegate.get_ptr().get() != nullptr);
  TEST_PROBE(P1030, EXPECT_TRUE(has_stored_pointer));
}
#endif
#ifdef UT03
TVOID TEST_UT03(DelegateUnitGTest, SimpleLambdaDelegate) {
  using CounterDelegate = Delegate<void(int*)>;
  int test_counter = 0;
  const int expected_count = 10;
  // [](int* ctr) { *ctr += 10; }
  class __lambda_155_26 {
   public:
    inline /*constexpr */ void operator()(int* ctr) const { *ctr = *ctr + 10; }

    using retType_155_26 = void (*)(int*);
    inline constexpr operator retType_155_26() const noexcept {
      return __invoke;
    }

   private:
    static inline /*constexpr */ void __invoke(int* ctr) {
      __lambda_155_26{}.operator()(ctr);
    }

   public:
    // inline /*constexpr */ __lambda_155_26(__lambda_155_26 &&) noexcept =
    // default;
    // /*constexpr */ __lambda_155_26() = default;
  };
  CounterDelegate delegate(__lambda_155_26{});
  delegate(&test_counter);
  TEST_PROBE(P1000, EXPECT_EQ(test_counter, expected_count));
  long use_count = delegate.get_ptr().use_count();
  long expected_use_count = 1;
  TEST_PROBE(P1010, EXPECT_EQ(use_count, expected_use_count));
  bool has_stored_pointer = (delegate.get_ptr().get() != nullptr);
  TEST_PROBE(P1020, EXPECT_TRUE(has_stored_pointer));
}
#endif
#ifdef UT04
/**
 * @test DelegateUnitGTest_UT4ComplexLambdaDelegate_Test
 *
 * @brief Test a more complex lambda delegate.
 *
 * @details Used cpp-insights to generate lambda type so we can see what is
 * going on.
 */
TVOID TEST_UT04(DelegateUnitGTest, ComplexLambdaDelegate) {
  struct TestFixture {
    int delegateFired;
    int count;
    // [this](int* ctr)->void{*ctr += 10; delegateFired++; };
    class __lambda_145_14 {
     public:
      inline /*constexpr */ void operator()(int* ctr) const {
        *ctr = *ctr + 10;
        __this->delegateFired++;
      }

     private:
      TestFixture* __this;

     public:
      __lambda_145_14(TestFixture* _this) : __this{_this} {}
    };
    Delegate<void(int*)> member_delegate;
    inline TestFixture()
        : delegateFired{0}, count{0}, member_delegate(__lambda_145_14{this}) {}
  };
  TestFixture fixture;
  fixture.member_delegate(&(fixture.count));
  TEST_PROBE(P1000, EXPECT_EQ(fixture.count, 10));
  TEST_PROBE(P1010, EXPECT_EQ(fixture.delegateFired, 1));
}
#endif
#ifdef UT05
/**
 * @test EdUnitGTest_UT5SubscriptionIdsFromDelegate_Test
 *
 * @brief Test for unique generated subscription ids
 */
TVOID TEST_UT05(DelegateUnitGTest, UniqueSubscriptionIdsFromDelegate) {
  using CounterDelegate = Delegate<void(int*)>;
  CounterDelegate dbtf(basicTestFunction);
  CounterDelegate dbdfn(BasicDelegateFunctionNoexcept);
  CounterDelegate dh(h);
  TEST_PROBE(P1000,
             EXPECT_NE(dbtf.get_Subscriber_ID(), dbdfn.get_Subscriber_ID()));
  TEST_PROBE(P1010,
             EXPECT_NE(dbtf.get_Subscriber_ID(), dh.get_Subscriber_ID()));
  TEST_PROBE(P1020,
             EXPECT_NE(dbdfn.get_Subscriber_ID(), dh.get_Subscriber_ID()));
}
#endif
void test_delegate(int* i) noexcept {
  *i += 1;
}
#ifdef UT06
TVOID TEST_UT06(DelegateUnitGTest, CheckRegularFunctionNoExcept) {
  Delegate<void(int*)> test_d(test_delegate);
  TEST_PROBE(P1000, EXPECT_FALSE(test_d.get_except_flag()));
}
#endif
void test_delegate_with_except(int* i) noexcept(false) {}
#ifdef UT07
TVOID TEST_UT07(DelegateUnitGTest, CheckRegularFunctionExcept) {
  Delegate<void(int*)> test_de(test_delegate_with_except);
  TEST_PROBE(P1000, EXPECT_TRUE(test_de.get_except_flag()));
}
#endif
#ifdef UT08
TVOID TEST_UT08(DelegateUnitGTest, CheckRegularFunctionNoExcept) {
  Delegate<void(int*, int)> test_d(
      [](int* ctr, int amt) noexcept -> void { *ctr += amt; });
  TEST_PROBE(P1000, EXPECT_FALSE(test_d.get_except_flag()));
}
#endif
#ifdef UT09
TVOID TEST_UT09(DelegateUnitGTest, CheckRegularFunctionExcept) {
  Delegate<void(int*, int)> test_d(
      [](int* ctr, int amt) noexcept(false) -> void { *ctr += amt; });
  TEST_PROBE(P1000, EXPECT_TRUE(test_d.get_except_flag()));
}
#endif
struct TestFixtureUT10 {
  int count = 0;
  void addne(int amount) noexcept(true) { count += amount; }
};
#ifdef UT10
/**
 * @test DelegateUnitGTest_UT10CheckMethodNoExcept_Test
 *
 * @brief Test for noexcept(true) condition.
 */
TVOID TEST_UT10(DelegateUnitGTest, CheckMethodNoExcept, UTDISABLED) {
  TestFixtureUT10 tf;
  Delegate<void(int)> test_d(tf, &TestFixtureUT10::addne);
  // Will always be true whether noexcept(true) or noexcept(false).
  TEST_PROBE(P1000, EXPECT_FALSE(test_d.get_except_flag()));
}
#endif
#ifdef UT11
struct TestFixtureUT11 {
  int count = 0;
  void addne(int amount) noexcept(false) { count += amount; }
};
TVOID TEST_UT11(DelegateUnitGTest, CheckMethodExcept) {
  TestFixtureUT11 tf;
  Delegate<void(int)> test_d(tf, &TestFixtureUT11::addne);
  TEST_PROBE(P1000, EXPECT_TRUE(test_d.get_except_flag()));
}
#endif
#ifdef UT12
struct TestFixtureUT12 {
  int count = 0;
  Delegate<void(int)> test_delegate;
  TestFixtureUT12()
      : test_delegate(
            [this](int amount) noexcept -> void { this->count += amount; }) {}
};
/** @test DelegateUnitGTest_UT10CheckComplexLambdaNoExcept_Test */
TVOID TEST_UT12(DelegateUnitGTest, CheckComplexLambdaNoExcept) {
  TestFixtureUT12 tf;
  TEST_PROBE(P1000, EXPECT_FALSE(tf.test_delegate.get_except_flag()));
}
#endif
#ifdef UT13
/**
 * @brief Test a lambda internal to a class.
 *
 * @bug note: see reference to function template instantiation
 * 'eventdelegatecpp::Delegate<void
 * (int)>::Delegate<TestFixtureUT13::{ctor}::<lambda_2_>>(T &&)' being compiled
 * with
 *        [
 *            T=TestFixtureUT13::{ctor}::<lambda_2_>
 *        ]
 * note: see the first reference to 'eventdelegatecpp::Delegate<void
 * (int)>::Delegate' in 'TestFixtureUT13::TestFixtureUT13'
 */
struct TestFixtureUT13 {
  int count = 0;
  Delegate<void(int)> test_delegate;
  TestFixtureUT13()
      : test_delegate([this](int amount) noexcept(false) -> void {
          this->count += amount;
        }) {}
};
/** @test DelegateUnitGTest_UT10CheckComplexLambdaExcept_Test */
TVOID TEST_UT13(DelegateUnitGTest, CheckComplexLambdaNoExcept) {
  TestFixtureUT13 tf;
  TEST_PROBE(P1000, EXPECT_TRUE(tf.test_delegate.get_except_flag()));
}
#endif
#ifdef UT14
TVOID TEST_UT14(DelegateUnitGTest, EmptyDelegateThrow) {
  try {
    throw_empty_delegate();
    TEST_PROBE(P1000A, FAIL());
  } catch (std::exception& e) {
    TEST_PROBE(P1000B, EXPECT_STREQ(e.what(), EmptyDelegateException));
  }
}
#endif
