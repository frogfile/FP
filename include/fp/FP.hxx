#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
namespace FP {
//
template <std::size_t ISizeBytes> class IPoint {
protected:
  std::array<uint8_t, ISizeBytes> _raw;

public:
  IPoint() = default;
  IPoint(IPoint &&) = default;
  IPoint(const IPoint &) = default;
  IPoint &operator=(IPoint &&) = default;
  IPoint &operator=(const IPoint &) = default;
  ~IPoint() = default;

  IPoint(std::array<uint8_t, ISizeBytes> raw) : _raw(raw) {}

  constexpr std::array<uint8_t, ISizeBytes> Raw() const { return _raw; };

  virtual operator double() const = 0;
  virtual IPoint &operator+(IPoint &rhs) const = 0;
  virtual IPoint &operator-(IPoint &rhs) const = 0;
  virtual IPoint &operator*(IPoint &rhs) const = 0;
  virtual IPoint &operator/(IPoint &rhs) const = 0;
  virtual IPoint &operator>(IPoint &rhs) const = 0;
  virtual IPoint &operator<(IPoint &rhs) const = 0;
  virtual IPoint &operator==(IPoint &rhs) const = 0;
};
} // namespace FP
