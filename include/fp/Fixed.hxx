#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <ostream>
#include <stdexcept>
#include <string>

#include "FP.hxx"

namespace FP {

// NOTE: DecBits runs out of numbers faster than FSizeBytes, so indexing all
// bits for any FSizeBytes bigger than UINT_MAX / 8 is impossible, but it
// doesn't matter since you'll run out of memory waaaay before it becomes an
// issue
template <size_t FSizeBytes = 8, size_t DecBits = 16>
class Fixed : public virtual IPoint<FSizeBytes> {
private:
  typedef IPoint<FSizeBytes> Base;

public:
  constexpr size_t Size() const override { return FSizeBytes; };
  constexpr size_t NumDecimalBits() const override { return DecBits; };
  constexpr size_t NumIntegerBits() const override {
    return FSizeBytes * 8 - DecBits;
  };

  Fixed(std::array<uint8_t, FSizeBytes> raw) : Base(raw) {
    if (FSizeBytes * 8 < DecBits)
      throw std::invalid_argument(
          "More decimal bits than the specified size allows");
  }

  Fixed() : Base() {};

  template <size_t RSize, size_t RDec> Fixed(Fixed<RSize, RDec> const &other) {
    if (!Fixed::Fits<RSize, RDec>())
      throw std::invalid_argument("Other is bigger than this");
    // TODO:
  };

  Fixed(double &val);

  constexpr std::bitset<FSizeBytes * 8> Bits() const {
    return static_cast<std::bitset<FSizeBytes * 8>>(this);
  }

  constexpr operator std::bitset<FSizeBytes * 8>() const {
    auto ret = std::bitset<FSizeBytes * 8>();

    for (size_t i = 0; i < FSizeBytes; i++) {
      ret <<= 8;
      ret |= this->_raw[i];
    }

    return ret;
  }

  // NOTE: Only works if the Fixed fits into double (duh)
  constexpr operator double() const override {
    if (FSizeBytes > sizeof(double))
      throw std::length_error("Fixed doesn't fit into double");

    double ret = 0;
    for (size_t i = 0; i < FSizeBytes * 8; i++) {
      if (this->_raw[i / 8] & (1 << (7 - i % 8)))
        // NOTE: since we only allow sizes that fit into double downcasting from
        // size_t to int64 will not overflow (i think)
        ret += std::pow(2, (int64_t)NumIntegerBits() - (int64_t)i - 1);
    }
    return ret;
  }

  // FIXME: Only works if the Fixed fits into double
  friend std::ostream &operator<<(std::ostream &out, Fixed const &fixed) {
    out << (double)fixed;
    return out;
  };

  // Widen left and right separately.
  /// @brief `Fixed` widened to fit Fixed<RSize,RDec>
  template <size_t RSize, size_t RDec>
  using RelaxedType =
      Fixed<FSizeBytes + (RDec - DecBits) / 8 +
                ((RSize * 8 - RDec) - (FSizeBytes * 8 - DecBits)) / 8,
            (RDec >= DecBits ? RDec : DecBits)>;

  /// @brief widen this to fit template arguments
  template <size_t RSize, size_t RDec>
  constexpr RelaxedType<RSize, RDec> relaxed() const {

    if (RSize < FSizeBytes)
      throw std::invalid_argument("New size is smaller than old size");

    auto ret = RelaxedType<RSize, RDec>();

    for (size_t i = 0; i < FSizeBytes; i++) {
      // FIXME: point not aligned actually
      ret[i + (ret.NumIntegerBits() - this->NumIntegerBits()) / 8] =
          this->_raw[i];
    }

    return ret;
  };

  // Check if this Fixed can fit Fixed<RSize,RDec> inside it
  template <size_t RSize, size_t RDec> static constexpr bool Fits() {
    return ((FSizeBytes * 8 - DecBits) >= RSize * 8 - RDec) &&
           (DecBits >= RDec);
  };

  // NOTE: lhs must fit rhs
  template <size_t RSize, size_t RDec>
  Fixed constexpr operator+(Fixed<RSize, RDec> const &rhs) const {

    if (!this->Fits(rhs) && !rhs.Fits(this))
      throw std::invalid_argument("Incompatible types");

    if (this->Fits(rhs)) {
      auto ret = Fixed<FSizeBytes, DecBits>();
      // TODO:
      return ret;
    }

    auto ret = Fixed<RSize, RDec>();
    // TODO:
    return ret;
  };

  // Fixed operator-(Fixed const &rhs) const override { /* TODO: */ };
  // Fixed operator*(Fixed const &rhs) const override { /* TODO: */ };
  // Fixed operator/(Fixed const &rhs) const override { /* TODO: */ };
  // bool operator>(Fixed const &rhs) const override { /* TODO: */ };
  // bool operator<(Fixed const &rhs) const override { /* TODO: */ };
  // bool operator==(Fixed const &rhs) const override { /* TODO: */ };
};
} // namespace FP
