#include <gtest/gtest.h>
#include "Delegate.h"
#include "util/ghelper.h"
#include "util/unit_test.h"
using namespace EDCPP::unit_test;
using namespace EDCPP;
// TODO test for void return requirement
namespace EDCPP::tests {
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

// ----------------------------------------------------------------------------
// TODO Test volatile GiftWrapper functions
// TODO Test const GiftWrapper functions
struct TestFixture1 {
  int executeCount = 0;
  bool delegateFired = false;
  explicit TestFixture1(int id) : executeCount(id) {
    // TODO Can't do this yet
    // ExecuteCount10 += Count10Reached;
  }
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
  // Event<void()> ExecuteCount10;
};

// TODO make separate tests with UT macros
template <typename T>
class DelegateFixture : public testing::Test {
 public:
  using CounterDelegate = Delegate<T>;
};

using BasicFunctionTypes = testing::Types<BasicDelegateFunction>;
TYPED_TEST_SUITE(DelegateFixture, BasicFunctionTypes);

TYPED_TEST(DelegateFixture, BasicFuntions) {
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
/**
 * @test EDUnitGTest_UT1Delegate_Test.
 */
TVOID TEST_UT1(EDUnitGTest, Delegate) {
  using CounterDelegate = Delegate<void(int*)>;
  int test_counter = 0;
  const int expected_count = 1;
  CounterDelegate delegate(basicTestFunction);
  delegate(&test_counter);
  TEST_PROBE(P1000, EXPECT_EQ(test_counter, expected_count));
  long use_count = delegate.get_ptr().use_count();
  long expected_use_count = 1;
  TEST_PROBE(P1010, EXPECT_EQ(use_count, expected_use_count));
  bool has_stored_pointer = (delegate.get_ptr().get() != nullptr);
  TEST_PROBE(P1020, EXPECT_TRUE(has_stored_pointer));
}
TVOID TEST_UT2(EDUnitGTest, MemberDelegate) {
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
TVOID TEST_UT3(EDUnitGTest, SimpleLambdaDelegate) {
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
/**
 * @test EDUnitGTest_UT4ComplexLambdaDelegate_Test
 *
 * @brief Test a more complex lambda delegate.
 *
 * @details Used cpp-insights to generate lambda type so we can see what is
 * going on.
 */
TVOID TEST_UT4(EDUnitGTest, ComplexLambdaDelegate) {
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
/**
 * @test EdUnitGTest_UT5SubscriptionIdsFromDelegate_Test
 *
 * @brief Test for unique generated subscription ids
 */
TVOID TEST_UT5(EDUnitGTest, UniqueSubscriptionIdsFromDelegate) {
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
void test_delegate(int* i) noexcept {
  *i += 1;
}
TVOID TEST_UT6(EDUnitGTest, CheckRegularFunctionNoExcept) {
  Delegate<void(int*)> test_d(test_delegate);
  TEST_PROBE(P1000, EXPECT_FALSE(test_d.get_except_flag()));
}
void test_delegate_with_except(int* i) noexcept(false) {}
TVOID TEST_UT7(EDUnitGTest, CheckRegularFunctionExcept) {
  Delegate<void(int*)> test_de(test_delegate_with_except);
  TEST_PROBE(P1000, EXPECT_TRUE(test_de.get_except_flag()));
}
TVOID TEST_UT8(EDUnitGTest, CheckRegularFunctionNoExcept) {
  Delegate<void(int*, int)> test_d(
      [](int* ctr, int amt) noexcept -> void { *ctr += amt; });
  TEST_PROBE(P1000, EXPECT_FALSE(test_d.get_except_flag()));
}
TVOID TEST_UT9(EDUnitGTest, CheckRegularFunctionExcept) {
  Delegate<void(int*, int)> test_d(
      [](int* ctr, int amt) noexcept(false) -> void { *ctr += amt; });
  TEST_PROBE(P1000, EXPECT_TRUE(test_d.get_except_flag()));
}
struct TestFixtureUT10 {
  int count = 0;
  void addne(int amount) noexcept(true) { count += amount; }
};
/**
 * @test EDUnitGTest_UT10CheckMethodNoExcept_Test
 *
 * @brief Test for noexcept(true) condition.
 */
TVOID TEST_UT10(EDUnitGTest, CheckMethodNoExcept, UTDISABLED) {
  TestFixtureUT10 tf;
  Delegate<void(int)> test_d(tf, &TestFixtureUT10::addne);
  // Will always be true whether noexcept(true) or noexcept(false).
  TEST_PROBE(P1000, EXPECT_FALSE(test_d.get_except_flag()));
}
struct TestFixtureUT11 {
  int count = 0;
  void addne(int amount) noexcept(false) { count += amount; }
};
TVOID TEST_UT11(EDUnitGTest, CheckMethodExcept) {
  TestFixtureUT11 tf;
  Delegate<void(int)> test_d(tf, &TestFixtureUT11::addne);
  TEST_PROBE(P1000, EXPECT_TRUE(test_d.get_except_flag()));
}
struct TestFixtureUT12 {
  int count = 0;
  Delegate<void(int)> test_delegate;
  TestFixtureUT12()
      : test_delegate(
            [this](int amount) noexcept -> void { this->count += amount; }) {}
};
/** @test EDUnitGTest_UT10CheckComplexLambdaNoExcept_Test */
TVOID TEST_UT12(EDUnitGTest, CheckComplexLambdaNoExcept) {
  TestFixtureUT12 tf;
  TEST_PROBE(P1000, EXPECT_FALSE(tf.test_delegate.get_except_flag()));
}
struct TestFixtureUT13 {
  int count = 0;
  Delegate<void(int)> test_delegate;
  TestFixtureUT13()
      : test_delegate([this](int amount) noexcept(false) -> void {
          this->count += amount;
        }) {}
};
/** @test EDUnitGTest_UT10CheckComplexLambdaExcept_Test */
TVOID TEST_UT13(EDUnitGTest, CheckComplexLambdaNoExcept) {
  TestFixtureUT13 tf;
  TEST_PROBE(P1000, EXPECT_TRUE(tf.test_delegate.get_except_flag()));
}
