#ifndef UNIT_TEST_H
#define UNIT_TEST_H
#include <string>
#include "ed_base.h"
#include "util/ghelper.h"

#ifndef EDCPP_STRINGIFY
#define EDCPP_STRINGIFY(s) #s
#endif
#ifndef EDCPP_FILELINE
#define EDCPP_FILELINE(line) __FILE__ ":" EDCPP_STRINGIFY(line)
#endif
#define EDCPP_UNIT_TEST_NAMESPACE EDCPP_COMBINE(unit_test)
#define EDCPP_UNIT_TEST_BEGIN namespace EDCPP_UNIT_TEST_NAMESPACE {
#define EDCPP_UNIT_TEST_END }

#define USING_EDCPP_UNIT_TEST using namespace EDCPP_UNIT_TEST_NAMESPACE;
// Make the list format better in VS
#define DECLARE_TEST_TYPE_COLLECTION(COLLECTION,...) \
  using COLLECTION = testing::Types < __VA_ARGS__ >

#endif
