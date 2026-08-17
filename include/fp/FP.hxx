#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#define CRTP_CHECK(base_t, derived_t)                                          \
  static_assert(std::is_base_of<base_t, derived_t>::value,                     \
                #base_t " CRTP is not derived from " #base_t)

#define CRTP_STUB_STATIC(base_t, derived_t, ret_t, name, ...)                  \
  static ret_t name(__VA_ARGS__) {                                             \
    CRTP_CHECK(base_t, derived_t);                                             \
    return derived_t::name(__VA_ARGS__);                                       \
  }

#define CRTP_STUB(base_t, derived_t, ret_t, name, ...)                         \
  ret_t name(__VA_ARGS__) {                                                    \
    CRTP_CHECK(base_t, derived_t);                                             \
    return derived_t.name(__VA_ARGS__);                                        \
  }

#define CRTP_OPERATOR(ret_t, sym)                                              \
  template <template <size_t RSize> class RHS>                                 \
  constexpr ret_t operator sym(RHS<ISizeBytes> const &rhs) const {             \
    static_assert(std::is_base_of<IPoint, RHS<ISizeBytes>>::value,             \
                  "RHS is not derived from IPoint");                           \
    return static_cast<Derived<ISizeBytes> *>(this)                            \
        ->template operator sym<RHS>(rhs);                                     \
  }

namespace FP {

template <std::size_t ISizeBytes,
          template <std::size_t DSize, std::size_t... DTempl> class Derived>
class IPoint {
private:
  constexpr static void check_derived() {
    static_assert(std::is_base_of<IPoint, Derived<ISizeBytes>>::value,
                  "IPoint Derived is not derived from IPoint");
  }

protected:
  std::array<uint8_t, ISizeBytes> _raw;

  IPoint() : _raw(std::array<uint8_t, ISizeBytes>()) {
    CRTP_CHECK(IPoint, Derived<ISizeBytes>);
  };
  IPoint(IPoint &&) { CRTP_CHECK(IPoint, Derived<ISizeBytes>); };
  IPoint(const IPoint &) { CRTP_CHECK(IPoint, Derived<ISizeBytes>); };

  IPoint &operator=(IPoint &&) = default;
  IPoint &operator=(const IPoint &) = default;
  ~IPoint() = default;

public:
  IPoint(std::array<uint8_t, ISizeBytes> raw) : _raw(raw) {}

  ///
  /// @section Static type data methods
  ///
  constexpr CRTP_STUB_STATIC(IPoint, Derived<ISizeBytes>, size_t, Size);
  constexpr CRTP_STUB_STATIC(IPoint, Derived<ISizeBytes>, size_t,
                             NumIntegerBits);
  constexpr CRTP_STUB_STATIC(IPoint, Derived<ISizeBytes>, size_t,
                             NumDecimalBits);

  template <std::size_t RSize, class RHS>
  static constexpr bool Fits(RHS const &rhs) {
    CRTP_CHECK(IPoint, Derived<ISizeBytes>);
    CRTP_CHECK(IPoint, RHS);

    return (NumIntegerBits() >= rhs.NumIntegerBits()) &&
           (NumDecimalBits() >= rhs.NumDecimalBits());
  };

  template <class RHS> static constexpr bool Fits() {
    CRTP_CHECK(IPoint, Derived<ISizeBytes>);
    CRTP_CHECK(IPoint, RHS);

    return (Derived<ISizeBytes>::NumIntegerBits() >= RHS::NumIntegerBits()) &&
           (Derived<ISizeBytes>::NumDecimalBits() >= RHS::NumDecimalBits());
  };

  template <std::size_t RNIB, std::size_t RNDB> static constexpr bool Fits() {
    CRTP_CHECK(IPoint, Derived<ISizeBytes>);

    return (NumIntegerBits() >= RNIB) && (NumDecimalBits() >= RNDB);
  };

  ///
  /// @section Raw byte array access
  ///

  constexpr std::array<uint8_t, ISizeBytes> Raw() const { return _raw; };
  constexpr explicit operator std::array<uint8_t, ISizeBytes>() const {
    return _raw;
  };

  constexpr typename std::array<uint8_t, ISizeBytes>::iterator Begin() const {
    return this->_raw.begin();
  }

  constexpr typename std::array<uint8_t, ISizeBytes>::iterator End() const {
    return this->_raw.end();
  }

  constexpr uint8_t &at(size_t idx) { return this->_raw.at(idx); };
  constexpr uint8_t &operator[](size_t idx) { return this->_raw[idx]; };

  ///
  /// @section Conversion functions
  ///

  constexpr explicit operator double() const {
    return static_cast<Derived<ISizeBytes> *>(this)->operator double();
  };

  ///
  /// @section Operators
  ///

  CRTP_OPERATOR(Derived<ISizeBytes>, +);
  CRTP_OPERATOR(Derived<ISizeBytes>, -);
  CRTP_OPERATOR(Derived<ISizeBytes>, *);
  CRTP_OPERATOR(Derived<ISizeBytes>, /);
  CRTP_OPERATOR(bool, >);
  CRTP_OPERATOR(bool, <);
  CRTP_OPERATOR(bool, ==);
};
}; // namespace FP

#undef CRTP_OPERATOR
#undef CRTP_STUB_STATIC
#undef CRTP_CHECK
