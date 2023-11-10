#include "Delegate.h"
#include <atomic>
EDCPP_USING
static_assert(std::atomic_uint64_t::is_always_lock_free);
// void EDCPP_CALL _MEDCPP::throw_empty_delegate() {
//   throw _MEDCPP::EmptyDelegate();
// }
