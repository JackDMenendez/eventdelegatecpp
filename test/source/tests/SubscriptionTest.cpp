#include <gtest/gtest.h>
#include "event_delegate.h"
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
  Event<void()> ExecuteCount10;
};

TVOID TEST(EDUnitGTest, StandardDelegate) {
  DirectEvent<void(int*)> event;

  int testCount = 0;
  int testCount2 = 0;
  const int fixtureInstance0_initialcount = 1;
  const int fixtureInstance1_initialcount = 2;
  TestFixture1 fixtureInstance0(fixtureInstance0_initialcount);
  TestFixture1 fixtureInstance1(fixtureInstance1_initialcount);

  event += basicTestFunction;              // testCount=1
  event += BasicDelegateFunctionNoexcept;  // testCount=3
  event += BasicDelegateFunctionNoexcept;  // testCount=5
  event += h;                              // testCount=8

  event += GiftWrapper(
      fixtureInstance0,
      &TestFixture1::increment);  // testCount=9
                                  // fixtureInstance0::executeCount=1
  event += member(
      fixtureInstance0,
      &TestFixture1::add2);  // testCount=11 fixtureInstance0::executeCount=3
  event += GiftWrapper(
      fixtureInstance1,
      &TestFixture1::increment);  // testCount=12
                                  // fixtureInstance1::executeCount=2
  event += member(
      fixtureInstance1,
      &TestFixture1::add2);  // testCount=14 fixtureInstance1::executeCount=4
  event += [](int* ctr) { *ctr += 10; };  // testCount=24
  // event += std::function<void(int*)>([=](int* ctr) {
  //     *ctr += amount_to_add;
  // });
  const int run1Expected_testCount = 24;
  const int run1Expected_fixtureInstance0 = 2;
  const int run1Expected_fixtureInstance1 = 3;
  event(&testCount);
  TEST_PROBE(P1000, EXPECT_TRUE(run1Expected_testCount == testCount));
  TEST_PROBE(P1010, EXPECT_TRUE(run1Expected_fixtureInstance0 ==
                                fixtureInstance0.executeCount));
  TEST_PROBE(P1020, EXPECT_TRUE(run1Expected_fixtureInstance1 ==
                                fixtureInstance1.executeCount));
  fixtureInstance0.executeCount = fixtureInstance0_initialcount;
  fixtureInstance1.executeCount = fixtureInstance1_initialcount;
  // removes all g from list
  event -= BasicDelegateFunctionNoexcept;  // testCount22
  event -= member(
      fixtureInstance0,
      &TestFixture1::add2);  // fixtureInstance0::executeCount=1 testCount=20
  event -= GiftWrapper(
      fixtureInstance1,
      &TestFixture1::increment);  // fixtureInstance1::executeCount=3
                                  // testCount=19
  const int run2Expected_testCount = 17;
  const int run2Expected_fixtureInstance0 = 2;
  const int run2Expected_fixtureInstance1 = 2;
  event(&testCount2);
  TEST_PROBE(P1030, EXPECT_TRUE(run2Expected_testCount == testCount2));
  TEST_PROBE(P1040, EXPECT_TRUE(run2Expected_fixtureInstance0 ==
                                fixtureInstance0.executeCount));
  TEST_PROBE(P1050, EXPECT_TRUE(run2Expected_fixtureInstance1 ==
                                fixtureInstance1.executeCount));
}
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
  CounterDelegate lambda_delegate(__lambda_155_26{});
  lambda_delegate(&test_counter);
  TEST_PROBE(P1000, EXPECT_EQ(test_counter, expected_count));
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
TVOID TEST_UT5(EDUnitGTest, SubscriptionIdsFromDelegate) {
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
