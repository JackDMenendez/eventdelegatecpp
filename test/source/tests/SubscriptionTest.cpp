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
  explicit TestFixture1(int id) : executeCount(id) {}
  void increment(int* ctr) {
    executeCount++;
    *ctr += 1;
  }
  void add2(int* ctr) {
    executeCount += 2;
    *ctr += 2;
  }
};

TVOID TEST_UT1(EDUnitGTest, StandardDelegate) {

  EventSubscription<void(int*)> event_subscription;

  int testCount = 0;
  int testCount2 = 0;
  const int fixtureInstance0_initialcount = 0;
  const int fixtureInstance1_initialcount = 1;
  TestFixture1 fixtureInstance0(fixtureInstance0_initialcount);
  TestFixture1 fixtureInstance1(fixtureInstance1_initialcount);

  event_subscription += f;  // testCount=1
  event_subscription += g;  // testCount=3
  event_subscription += g;  // testCount=5
  event_subscription += h;  // testCount=8

  event_subscription +=
      member(fixtureInstance0,
             &TestFixture1::increment);  // testCount=9
                                         // fixtureInstance0::executeCount=1
  event_subscription += member(
      fixtureInstance0,
      &TestFixture1::add2);  // testCount=11 fixtureInstance0::executeCount=3
  event_subscription +=
      member(fixtureInstance1,
             &TestFixture1::increment);  // testCount=12
                                         // fixtureInstance1::executeCount=2
  event_subscription += member(
      fixtureInstance1,
      &TestFixture1::add2);  // testCount=14 fixtureInstance1::executeCount=4
  event_subscription += [](int* ctr) { *ctr += 10; };  // testCount=24
  // event_subscription += std::function<void(int*)>([=](int* ctr) {
  //     *ctr += amount_to_add;
  // });
  const int run1Expected_testCount = 24;
  const int run1Expected_fixtureInstance0 = 3;
  const int run1Expected_fixtureInstance1 = 4;
  event_subscription(&testCount);
  TEST_PROBE(P1000,EXPECT_TRUE(run1Expected_testCount == testCount));
  TEST_PROBE(P1010,EXPECT_TRUE(run1Expected_fixtureInstance0 == fixtureInstance0.executeCount));
  TEST_PROBE(P1020,EXPECT_TRUE(run1Expected_fixtureInstance1 == fixtureInstance1.executeCount));
  fixtureInstance0.executeCount = fixtureInstance0_initialcount;
  fixtureInstance1.executeCount = fixtureInstance1_initialcount;
  // removes all g from list
  event_subscription -= g;  // testCount22
  event_subscription -= member(
      fixtureInstance0,
      &TestFixture1::add2);  // fixtureInstance0::executeCount=1 testCount=20
  event_subscription -=
      member(fixtureInstance1,
             &TestFixture1::increment);  // fixtureInstance1::executeCount=3
                                         // testCount=19
  const int run2Expected_testCount = 17;
  const int run2Expected_fixtureInstance0 = 1;
  const int run2Expected_fixtureInstance1 = 3;
  event_subscription(&testCount2);
  EXPECT_TRUE(run2Expected_testCount == testCount2);
  EXPECT_TRUE(run2Expected_fixtureInstance0 == fixtureInstance0.executeCount);
  EXPECT_TRUE(run2Expected_fixtureInstance1 == fixtureInstance1.executeCount);
}
