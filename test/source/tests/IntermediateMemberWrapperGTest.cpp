
#include <gtest/gtest.h>
#define MTESTREGIME 1
#include "IntermediateMemberWrapper.h"
#include "MethodWrapper.h"
#include "util/ghelper.h"
#include "util/unit_test.h"
EDCPP_USING
// TODO volatile class tests
struct IIntMethodWrapperTestFixture {
  virtual void testMethodExceptNoConstNoVolatile(int amt) = 0;
  virtual void testMethodNoExceptNoConstNoVolatile(int amt) noexcept = 0;
  virtual void testMethodExceptConstNoVolatile() const = 0;
  virtual void testMethodNoExceptConstNoVolatile() const noexcept = 0;
  virtual ~IIntMethodWrapperTestFixture() noexcept {}
};
struct IntMethodWrapperTestFixture : public IIntMethodWrapperTestFixture {
  int total = 0;
  mutable int mutable_total = 0;
  volatile int vol_total = 0;
  void testMethodExceptNoConstNoVolatile(int amt) override {
    total += amt;
    mutable_total++;
  }
  void testMethodNoExceptNoConstNoVolatile(int amt) noexcept override {
    total += amt;
    mutable_total++;
  }
  void testMethodExceptConstNoVolatile() const override { mutable_total++; }
  void testMethodNoExceptConstNoVolatile() const noexcept override {
    mutable_total++;
  }

  ~IntMethodWrapperTestFixture() noexcept override {}
};
// TODO Use the following 3 UT to create UT similar UT delegate tests.
TVOID TEST_UT01(IntermediateMethodWrapperUnitGTest, ExceptNoConstNoVolatile) {
  IntMethodWrapperTestFixture testFixture;
  const int expected_value = 2;
  const int amt_to_add_param = 2;
  const int expected_volatile = 0;
  const int expected_mutable = 1;

  auto wrappedMethod = MethodWrapper(
      testFixture,
      &IntMethodWrapperTestFixture::testMethodExceptNoConstNoVolatile);
  wrappedMethod(amt_to_add_param);
  TEST_PROBE(P1000, EXPECT_EQ(expected_value, testFixture.total));
  TEST_PROBE(P1005, EXPECT_EQ(expected_mutable, testFixture.mutable_total));
  TEST_PROBE(P1007, EXPECT_EQ(expected_volatile, testFixture.vol_total));
  TEST_PROBE(P1010, EXPECT_FALSE(wrappedMethod.m_const));
  TEST_PROBE(P1020, EXPECT_FALSE(wrappedMethod.m_volatile));
  TEST_PROBE(P1030, EXPECT_FALSE(wrappedMethod.m_noexcept));
}
TVOID TEST_UT02(IntermediateMethodWrapperUnitGTest, NoExceptNoConstNoVolatile) {
  IntMethodWrapperTestFixture testFixture;
  const int expected_value = 2;
  const int amt_to_add_param = 2;
  const int expected_volatile = 0;
  const int expected_mutable = 1;

  auto wrappedMethod = MethodWrapper(
      testFixture,
      &IntMethodWrapperTestFixture::testMethodNoExceptNoConstNoVolatile);
  wrappedMethod(amt_to_add_param);
  TEST_PROBE(P1000, EXPECT_EQ(expected_value, testFixture.total));
  TEST_PROBE(P1005, EXPECT_EQ(expected_mutable, testFixture.mutable_total));
  TEST_PROBE(P1007, EXPECT_EQ(expected_volatile, testFixture.vol_total));
  TEST_PROBE(P1010, EXPECT_FALSE(wrappedMethod.m_const));
  TEST_PROBE(P1020, EXPECT_FALSE(wrappedMethod.m_volatile));
  TEST_PROBE(P1030, EXPECT_TRUE(wrappedMethod.m_noexcept));
}
TVOID TEST_UT03(IntermediateMethodWrapperUnitGTest, ExceptConstNoVolatile) {
  IntMethodWrapperTestFixture testFixture;
  const int expected_value = 0;
  const int expected_mutable = 1;
  const int expected_volatile = 0;
  auto wrappedMethod = MethodWrapper(
      testFixture,
      &IntMethodWrapperTestFixture::testMethodExceptConstNoVolatile);
  wrappedMethod();
  TEST_PROBE(P1000, EXPECT_EQ(expected_value, testFixture.total));
  TEST_PROBE(P1005, EXPECT_EQ(expected_mutable, testFixture.mutable_total));
  TEST_PROBE(P1007, EXPECT_EQ(expected_volatile, testFixture.vol_total));
  TEST_PROBE(P1010, EXPECT_TRUE(wrappedMethod.m_const));
  TEST_PROBE(P1020, EXPECT_FALSE(wrappedMethod.m_volatile));
  TEST_PROBE(P1030, EXPECT_FALSE(wrappedMethod.m_noexcept));
}
TVOID TEST_UT04(IntermediateMethodWrapperUnitGTest, NoExceptConstNoVolatile) {
  IntMethodWrapperTestFixture testFixture;
  const int expected_value = 0;
  const int expected_mutable = 1;
  const int expected_volatile = 0;
  auto wrappedMethod = MethodWrapper(
      testFixture,
      &IntMethodWrapperTestFixture::testMethodNoExceptConstNoVolatile);
  wrappedMethod();
  TEST_PROBE(P1000, EXPECT_EQ(expected_value, testFixture.total));
  TEST_PROBE(P1005, EXPECT_EQ(expected_mutable, testFixture.mutable_total));
  TEST_PROBE(P1007, EXPECT_EQ(expected_volatile, testFixture.vol_total));
  TEST_PROBE(P1010, EXPECT_TRUE(wrappedMethod.m_const));
  TEST_PROBE(P1020, EXPECT_FALSE(wrappedMethod.m_volatile));
  TEST_PROBE(P1030, EXPECT_TRUE(wrappedMethod.m_noexcept));
}
