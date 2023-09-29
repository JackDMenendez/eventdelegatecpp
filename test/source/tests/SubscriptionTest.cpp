#include <gtest/gtest.h>
#include "event_delegate.h"
#include "util/ghelper.h"
#include "util/unit_test.h"
using namespace EDCPP::unit_test;
using namespace EDCPP;

namespace EDCPP::tests {
void test_base_subscription_scenarios();
}

void f(int* ctr) {
  *ctr += 1;
}
void g(int* ctr) {
  *ctr += 2;
}
void h(int* ctr) {
  *ctr += 3;
}

// ----------------------------------------------------------------------------

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
  void add2(int* ctr) {
    executeCount += 2;
    *ctr += 2;
    if (executeCount == 10) {
      onCount10();
    }
  }
  void Count10Reached() {
    delegateFired = true;
    MMSG("delegate fired");
  }
  virtual void onCount10() {}
  Event<void()> ExecuteCount10;
};

TVOID TEST(EDUnitGTest, StandardDelegate) {
  Event<void(int*)> event;

  int testCount = 0;
  int testCount2 = 0;
  const int fixtureInstance0_initialcount = 0;
  const int fixtureInstance1_initialcount = 1;
  TestFixture1 fixtureInstance0(fixtureInstance0_initialcount);
  TestFixture1 fixtureInstance1(fixtureInstance1_initialcount);

  event += f;  // testCount=1
  event += g;  // testCount=3
  event += g;  // testCount=5
  event += h;  // testCount=8

  event +=
      member(fixtureInstance0,
             &TestFixture1::increment);  // testCount=9
                                         // fixtureInstance0::executeCount=1
  event += member(
      fixtureInstance0,
      &TestFixture1::add2);  // testCount=11 fixtureInstance0::executeCount=3
  event +=
      member(fixtureInstance1,
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
  const int run1Expected_fixtureInstance0 = 3;
  const int run1Expected_fixtureInstance1 = 4;
  event(&testCount);
  TEST_PROBE(P1000, EXPECT_TRUE(run1Expected_testCount == testCount));
  TEST_PROBE(P1010, EXPECT_TRUE(run1Expected_fixtureInstance0 ==
                                fixtureInstance0.executeCount));
  TEST_PROBE(P1020, EXPECT_TRUE(run1Expected_fixtureInstance1 ==
                                fixtureInstance1.executeCount));
  fixtureInstance0.executeCount = fixtureInstance0_initialcount;
  fixtureInstance1.executeCount = fixtureInstance1_initialcount;
  // removes all g from list
  event -= g;  // testCount22
  event -= member(
      fixtureInstance0,
      &TestFixture1::add2);  // fixtureInstance0::executeCount=1 testCount=20
  event -=
      member(fixtureInstance1,
             &TestFixture1::increment);  // fixtureInstance1::executeCount=3
                                         // testCount=19
  const int run2Expected_testCount = 17;
  const int run2Expected_fixtureInstance0 = 1;
  const int run2Expected_fixtureInstance1 = 3;
  event(&testCount2);
  TEST_PROBE(P1030, EXPECT_TRUE(run2Expected_testCount == testCount2));
  TEST_PROBE(P1040, EXPECT_TRUE(run2Expected_fixtureInstance0 ==
                                fixtureInstance0.executeCount));
  TEST_PROBE(P1050, EXPECT_TRUE(run2Expected_fixtureInstance1 ==
                                fixtureInstance1.executeCount));
}
/**
 * @test EDUnitGTest_Delegate_UT1_Test.
 */
TVOID TEST_UT1(EDUnitGTest, Delegate) {
  using CounterDelegate = Delegate<void(int*)>;
  int test_counter = 0;
  const int expected_count = 1;
  CounterDelegate delegate(f);
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
  CounterDelegate lambda_delegate([](int* ctr) { *ctr += 10; });
  lambda_delegate(&test_counter);
  TEST_PROBE(P1000, EXPECT_EQ(test_counter, expected_count));
}
