#ifndef EVENTDELEGATECPP_ED_H
#define EVENTDELEGATECPP_ED_H
#include <cstddef>
#include <string>
#include "forward_decl.h"
#include "ed_defs.h"
// Provide the root namespace.
#ifndef EDCPP_STRINGIFY
#define EDCPP_STRINGIFY(s) #s
#endif
#ifndef EDCPP_FILELINE
#define EDCPP_FILELINE(line) __FILE__ ":" EDCPP_STRINGIFY(line)
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
