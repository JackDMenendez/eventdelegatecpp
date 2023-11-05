#ifndef EVENTDELEGATECPP_DEFAULT_SENDER_HPP
#define EVENTDELEGATECPP_DEFAULT_SENDER_HPP
#include "ed_base.h"
EDCPP_BEGIN
struct Info {
 public:
  virtual ~Info() noexcept = default;
  Info() noexcept = default;
  virtual bool operator==(const Info& other) const noexcept { return false; }
};
struct Sender {
 public:
  virtual ~Sender() noexcept = default;
  Sender() noexcept = default;
  virtual bool operator==(const Sender& other) const noexcept { return false; }
};
static Sender defaultSender;
static Info defaultInfo;
EDCPP_END
#endif
