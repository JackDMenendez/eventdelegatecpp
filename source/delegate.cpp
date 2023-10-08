#include "Delegate.h"
#include <atomic>
static_assert(std::atomic_uint64_t::is_always_lock_free);
