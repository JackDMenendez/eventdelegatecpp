#ifndef IDELEGATESHAREDWRAPPER_H
#define IDELEGATESHAREDWRAPPER_H
#include <memory>
#include "ed_base.h"
#include "ICallBackWrapper.h"
EDCPP_BEGIN
template <typename... Args>
struct IDelegateSharedWrapper {
  virtual std::shared_ptr<ICallBackWrapper<void(Args...)>>&
  get_ptr() noexcept = 0;
  virtual ~IDelegateSharedWrapper() noexcept = default;
};
EDCPP_END
#endif
