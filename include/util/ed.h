#ifndef EVENTDELEGATECPP_ED_H
#define EVENTDELEGATECPP_ED_H
#include <cstddef>
#include <string>
#include "ed_defs.h"
#include "forward.h"
// Provide the root namespace.
#define EDCPP eventdelegatecpp
#ifndef STRINGIFY
#define STRINGIFY(s) #s
#endif
#ifndef FILELINE
#define FILELINE(line) __FILE__ ":" STRINGIFY(line)
#endif
/**
 * String sugar, Literal class type that wraps a constant expression string.
 *
 * Uses implicit conversion to allow templates to *seemingly* accept constant
 * strings.
 */
template <size_t N>
struct StringTParm {
  constexpr StringTParm(const char (&str)[N]) { std::copy_n(str, N, value); }
  char value[N];
};

#endif
