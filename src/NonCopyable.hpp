#pragma once

namespace hst {

struct NonCopyable {
protected:
  constexpr NonCopyable() = default;
  ~NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

} //namespace hst
