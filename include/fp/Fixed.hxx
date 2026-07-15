#pragma once

#include <array>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ostream>

#include "FP.hxx"

namespace FP {

template <size_t FSizeBytes, size_t DecBits>
class Fixed : public virtual IPoint<FSizeBytes> {

public:
  static const size_t Size = FSizeBytes;
  static const size_t NumDecimalBits = DecBits;
  static const size_t NumIntegerBits = FSizeBytes * 8 - DecBits;

  Fixed(std::array<uint8_t, FSizeBytes> raw) : IPoint<FSizeBytes>(raw) {};
  Fixed() : IPoint<FSizeBytes>() {};

  Fixed(double &val);

  constexpr typename std::array<uint8_t, FSizeBytes>::iterator Begin() const {
    return this->_raw.begin();
  }

  constexpr typename std::array<uint8_t, FSizeBytes>::iterator End() const {
    return this->_raw.end();
  }

  constexpr operator std::bitset<FSizeBytes * 8>() const {
    auto ret = std::bitset<FSizeBytes * 8>();

    for (size_t i = 0; i < FSizeBytes; i++) {
      ret <<= 8;
      ret |= this->_raw[i];
    }

    return ret;
  }

  // FIXME: Only works if the Fixed fits into double
  constexpr operator double() const override {
    double ret = 0;
    for (size_t i = 0; i < FSizeBytes * 8; i++) {
      if (this->_raw[i / 8] & (1 << (7 - i % 8)))
        ret += std::pow(2, (int64_t)NumIntegerBits - (int64_t)i - 1);
    }
    return ret;
  }

  // FIXME: Only works if the Fixed fits into double
  friend std::ostream &operator<<(std::ostream &out, Fixed const &fixed) {
    out << (double)fixed;
    return out;
  };

  // Fixed &operator+(IPoint<FSizeBytes> const &rhs) const override {
  //   return Fixed();
  // };
  //
  // Fixed &
  // operator-(IPoint<FSizeBytes> const &rhs) const override { /* TODO: */ };
  // Fixed &
  // operator*(IPoint<FSizeBytes> const &rhs) const override { /* TODO: */ };
  // Fixed &
  // operator/(IPoint<FSizeBytes> const &rhs) const override { /* TODO: */ };
  // Fixed &
  // operator>(IPoint<FSizeBytes> const &rhs) const override { /* TODO: */ };
  // Fixed &
  // operator<(IPoint<FSizeBytes> const &rhs) const override { /* TODO: */ };

  // Fixed &
  // operator==(IPoint<FSizeBytes> const &rhs) const override { /* TODO: */ };
};
} // namespace FP
