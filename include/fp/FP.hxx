#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
namespace FP {
//
template <std::size_t ISizeBytes, class Derived> class IPoint {
protected:
  std::array<uint8_t, ISizeBytes> _raw;

  IPoint() = default;
  IPoint(IPoint &&) = default;
  IPoint(const IPoint &) = default;
  IPoint &operator=(IPoint &&) = default;
  IPoint &operator=(const IPoint &) = default;
  ~IPoint() = default;

public:
  static constexpr size_t Size() { return Derived::Size(); };
  static constexpr size_t NumDecimalBits() {
    return Derived::NumDecimalBits();
  };
  static constexpr size_t NumIntegerBits() {
    return Derived::NumIntegerBits();
  };

  template <std::size_t RSize, class RD>
  static constexpr bool Fits(IPoint<RSize, RD> const &rhs) {
    return (NumIntegerBits() >= rhs.NumIntegerBits()) &&
           (NumDecimalBits() >= rhs.NumDecimalBits());
  };

  template <class RHS> static constexpr bool Fits() {
    return (NumIntegerBits() >= RHS::NumIntegerBits()) &&
           (NumDecimalBits() >= RHS::NumDecimalBits());
  };

  template <std::size_t RNIB, std::size_t RNDB> static constexpr bool Fits() {
    return (NumIntegerBits() >= RNIB) && (NumDecimalBits() >= RNDB);
  };

  IPoint(std::array<uint8_t, ISizeBytes> raw) : _raw(raw) {}

  constexpr std::array<uint8_t, ISizeBytes> Raw() const { return _raw; };
  constexpr operator std::array<uint8_t, ISizeBytes>() const { return _raw; };

  constexpr typename std::array<uint8_t, ISizeBytes>::iterator Begin() const {
    return this->_raw.begin();
  }

  constexpr typename std::array<uint8_t, ISizeBytes>::iterator End() const {
    return this->_raw.end();
  }

  constexpr uint8_t &at(size_t idx) { return this->_raw.at(idx); };
  constexpr uint8_t &operator[](size_t idx) { return this->_raw[idx]; };

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
