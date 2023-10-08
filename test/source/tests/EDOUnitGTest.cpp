
#include <gtest/gtest.h>
#include "first_try.h"
#include "gtest/gtest-spi.h"
#include "util/ghelper.h"
#include "util/unit_test.h"
using namespace EDCPP::unit_test;
using namespace ED;
void basicTestFunctionWW(int* ctr) {
  *ctr += 1;
}
using BasicDelegateFunction = void(int*);  // void __cdecl(int * __ptr64)
/**
 * For testing with noexcept
 *
 * The noexcept keyword does not change a function type but we test for it
 * anyway so we know if something changes.
 */
void BasicDelegateFunctionWW(int* ctr) noexcept {
  *ctr += 2;
}
void hWW(int* ctr) {
  *ctr += 3;
}
struct TestFixtureWW {
  int executeCount = 0;
  bool dead = false;
  bool delegateFired = false;
  explicit TestFixtureWW(int id) : executeCount(id) {
    // TODO Can't do this yet
    // ExecuteCount10 += Count10Reached;
  }
  void increment(int* ctr) {
    // There are legitimate cases where dead is false
    /*
    if (dead) {
      EXPECT_NONFATAL_FAILURE(EXPECT_FALSE(dead),
                              "Increment is called on an object that has a "
                              "destructor previously called");
    }
    */
    executeCount++;
    *ctr += 1;
    if (executeCount == 10) {
      onCount10();
    }
  }
  void add2(int* ctr) const {
    EXPECT_FALSE(dead);
    *ctr += 2;
  }
  void Count10Reached() {
    delegateFired = true;
    MMSG("delegate fired");
  }
  virtual void onCount10() {}
  DirectEvent<void()> ExecuteCount10;
  virtual ~TestFixtureWW() { dead = true; }
};
TVOID TEST_UT1(EDOUnitGTest, StandardDelegate) {
  DirectEvent<void(int*)> event;

  int testCount = 0;
  int testCount2 = 0;
  const int fixtureInstance0_initialcount = 1;
  const int fixtureInstance1_initialcount = 2;
  TestFixtureWW fixtureInstance0(fixtureInstance0_initialcount);
  TestFixtureWW fixtureInstance1(fixtureInstance1_initialcount);

  event += basicTestFunctionWW;      // testCount=1
  event += BasicDelegateFunctionWW;  // testCount=3
  event += BasicDelegateFunctionWW;  // testCount=5
  event += hWW;                      // testCount=8

  event +=
      member(fixtureInstance0,
             &TestFixtureWW::increment);  // testCount=9
                                          // fixtureInstance0::executeCount=1
  event += member(
      fixtureInstance0,
      &TestFixtureWW::add2);  // testCount=11 fixtureInstance0::executeCount=3
  event +=
      member(fixtureInstance1,
             &TestFixtureWW::increment);  // testCount=12
                                          // fixtureInstance1::executeCount=2
  event += member(
      fixtureInstance1,
      &TestFixtureWW::add2);  // testCount=14 fixtureInstance1::executeCount=4
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
  event -= BasicDelegateFunctionWW;  // testCount22
  event -= member(
      fixtureInstance0,
      &TestFixtureWW::add2);  // fixtureInstance0::executeCount=1 testCount=20
  event -=
      member(fixtureInstance1,
             &TestFixtureWW::increment);  // fixtureInstance1::executeCount=3
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
void UseTheEvent(DirectEvent<void(int*)>& event, int* testCount) {
  TestFixtureWW fixtureInstance0(0);
  TEST_PROBE(P0005, EXPECT_EQ(fixtureInstance0.executeCount, 0));
  event +=
      member(fixtureInstance0,
             &TestFixtureWW::increment);  // testCount=9
                                          // fixtureInstance0::executeCount=1
  event(testCount);
  TEST_PROBE(P1002, EXPECT_EQ(fixtureInstance0.executeCount, 1));
}
void UseTheEventAgain(DirectEvent<void(int*)>& event, int* testCount) {
  uint8_t buffer[512];
  for (int i = 0; i < 512; i++) {
    buffer[i] = 0;
  }
  // Fixture instance 1 not added to event
  event(testCount);
  // ExecuteCount should not be incremented.
  for (int i = 0; i < 512; i++) {
    if (buffer[i] != 0) {
      MINFO << "Offset: " << i << std::endl;
      TEST_PROBE(P1005, EXPECT_EQ(buffer[i], 0));
    }
  }
}
TVOID TEST_UT2(EDOUnitGTest, StandardDelegateError, UTDISABLED) {
  const int fixtureInstance0_initialcount = 1;
  int testCount = 0;
  DirectEvent<void(int*)> event;
  UseTheEvent(event, &testCount);
  TEST_PROBE(P1000, EXPECT_TRUE(1 == testCount));
  UseTheEventAgain(event, &testCount);
  TEST_PROBE(P1020, EXPECT_TRUE(2 == testCount));
}
