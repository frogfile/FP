#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <ostream>

#include "FP.hxx"

namespace FP {

template <size_t FSizeBytes = 8, size_t DecBits = 16>
class Fixed : public virtual IPoint<FSizeBytes> {
private:
  typedef IPoint<FSizeBytes> Base;

public:
  static constexpr size_t Size = FSizeBytes;
  static constexpr size_t NumDecimalBits = DecBits;
  static constexpr size_t NumIntegerBits = FSizeBytes * 8 - DecBits;

  Fixed(std::array<uint8_t, FSizeBytes> raw) : Base(raw) {
    if (FSizeBytes * 8 < DecBits)
      throw std::invalid_argument(
          "More decimal bits than the specified size allows");
  }

  Fixed() : Base() {};

  template <size_t RSize, size_t RDec>
  Fixed(Fixed<RSize, RDec> const &other){

  };

  Fixed(double &val);

  constexpr operator std::bitset<FSizeBytes * 8>() const {
    auto ret = std::bitset<FSizeBytes * 8>();

    for (size_t i = 0; i < FSizeBytes; i++) {
      ret <<= 8;
      ret |= this->_raw[i];
    }

    return ret;
  }

  constexpr std::bitset<FSizeBytes * 8> Bits() const {
    return static_cast<std::bitset<FSizeBytes * 8>>(this);
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

  // Widen left and right separately.
  // `Fixed` widened to fit Fixed<RSize,RDec>
  template <size_t RSize, size_t RDec>
  using RelaxedType = Fixed<FSizeBytes + (RDec - DecBits) / 8 +
                                ((RSize * 8 - RDec) - NumIntegerBits) / 8,
                            (RDec >= DecBits ? RDec : DecBits)>;

  // NOTE: widen this to fit template arguments
  template <size_t RSize, size_t RDec>
  constexpr RelaxedType<RSize, RDec> relaxed() const {

    if (RSize < FSizeBytes)
      throw std::invalid_argument("New size is smaller than old size");

    auto ret = RelaxedType<RSize, RDec>();

    for (size_t i = 0; i < FSizeBytes; i++) {
      ret[i + (ret.NumIntegerBits - NumIntegerBits) / 8] = this->_raw[i];
    }

    return ret;
  };

  // Check if this Fixed can fit Fixed<RSize,RDec> inside it
  template <size_t RSize, size_t RDec> static constexpr bool Fits() {
    return (NumIntegerBits >= RSize * 8 - RDec) && (DecBits >= RDec);
  };

  // Check if this Fixed can fit T inside it
  template <class T> static constexpr bool Fits() {
    return (NumIntegerBits >= T::NumIntegerbits) &&
           (NumDecimalBits >= T::NumDecimalBits);
  };

  template <class T> static constexpr bool Fits(T const &&rhs) {
    return (NumIntegerBits >= T::NumIntegerbits) &&
           (NumDecimalBits >= T::NumDecimalBits);
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
