#include "Delegate.h"
#include <atomic>
EDCPP_USING
static_assert(std::atomic_uint64_t::is_always_lock_free);
// void EDCPP_CALL MEDCPP::throw_empty_delegate() {
//   throw MEDCPP::EmptyDelegate();
// }
