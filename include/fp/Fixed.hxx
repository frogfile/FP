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
  Fixed(std::array<uint8_t, FSizeBytes> raw) : IPoint<FSizeBytes>(raw) {};
  Fixed() : IPoint<FSizeBytes>() {};

  Fixed(double &val);

  constexpr uint16_t IntegerPart() const {
    // TODO:
    return static_cast<uint16_t>(this->_raw >> NumDecimalBits());
  }

  constexpr uint16_t DecimalPartRaw() const {
    // TODO:
    return static_cast<uint16_t>(this->_raw & 0x0000'FFFF);
  }

  constexpr operator std::bitset<FSizeBytes * 8>() const {
    auto ret = std::bitset<FSizeBytes * 8>();

    for (size_t i = 0; i < this->_raw.size(); i++) {
      ret <<= 8;
      ret |= this->_raw;
    }

    return ret;
  }

  // FIXME: Only works if the Fixed fits into double
  constexpr operator double() const override {
    double ret = 0;
    for (size_t i = 0; i < FSizeBytes * 8; i++) {
      if (this->_raw[i / 8] & (1 << (7 - i % 8)))
        ret += std::pow(2, (int64_t)NumIntegerBits() - (int64_t)i - 1);
    }
    return ret;
  }

  friend std::ostream &operator<<(std::ostream &out, Fixed const &fixed) {
    out << (std::bitset<FSizeBytes * 8>)fixed;
    return out;
  };

  Fixed &operator+(IPoint<FSizeBytes> &rhs) const override { /* TODO: */ };
  Fixed &operator-(IPoint<FSizeBytes> &rhs) const override { /* TODO: */ };
  Fixed &operator*(IPoint<FSizeBytes> &rhs) const override { /* TODO: */ };
  Fixed &operator/(IPoint<FSizeBytes> &rhs) const override { /* TODO: */ };
  Fixed &operator>(IPoint<FSizeBytes> &rhs) const override { /* TODO: */ };
  Fixed &operator<(IPoint<FSizeBytes> &rhs) const override { /* TODO: */ };
  Fixed &operator==(IPoint<FSizeBytes> &rhs) const override { /* TODO: */ };

  static constexpr size_t NumDecimalBits() { return DecBits; }

  static constexpr size_t NumIntegerBits() {
    // NOLINTNEXTLINE
    return static_cast<uint8_t>(FSizeBytes * 8 - DecBits);
  }
};
} // namespace FP
