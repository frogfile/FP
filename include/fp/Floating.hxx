#pragma once

#include "FP.hxx"

namespace FP {

template <size_t FSizeBytes>
class Floating : public IPoint<sizeof(float), Floating> {
public:
  Floating() = default;
  Floating(Floating &&) = default;
  Floating(const Floating &) = default;
  Floating &operator=(Floating &&) = default;
  Floating &operator=(const Floating &) = default;
  ~Floating() = default;

private:
};
} // namespace FP
