#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
namespace FP {
//
template <std::size_t ISizeBytes> class IPoint {
protected:
  std::array<uint8_t, ISizeBytes> _raw;

  IPoint() = default;
  IPoint(IPoint &&) = default;
  IPoint(const IPoint &) = default;
  IPoint &operator=(IPoint &&) = default;
  IPoint &operator=(const IPoint &) = default;
  ~IPoint() = default;

public:
  IPoint(std::array<uint8_t, ISizeBytes> raw) : _raw(raw) {}

  constexpr std::array<uint8_t, ISizeBytes> Raw() const { return _raw; };
  constexpr operator std::array<uint8_t, ISizeBytes>() const { return _raw; };

  constexpr typename std::array<uint8_t, ISizeBytes>::iterator Begin() const {
    return this->_raw.begin();
  }

  constexpr typename std::array<uint8_t, ISizeBytes>::iterator End() const {
    return this->_raw.end();
  }

  virtual operator double() const = 0;

  // virtual Derived operator+(Derived const &rhs) const = 0;
  // virtual Derived operator-(Derived const &rhs) const = 0;
  // virtual Derived operator*(Derived const &rhs) const = 0;
  // virtual Derived operator/(Derived const &rhs) const = 0;
  // virtual bool operator>(Derived const &rhs) const = 0;
  // virtual bool operator<(Derived const &rhs) const = 0;
  // virtual bool operator==(Derived const &rhs) const = 0;
};
} // namespace FP
