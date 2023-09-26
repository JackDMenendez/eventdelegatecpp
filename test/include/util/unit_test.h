#ifndef UNIT_TEST_H
#define UNIT_TEST_H
#include <string>
#include "ed_base.h"

#ifndef STRINGIFY
#define STRINGIFY(s) #s
#endif
#ifndef FILELINE
#define FILELINE(line) __FILE__ ":" STRINGIFY(line)
#endif

namespace EDCPP::unit_test {
}
#endif
