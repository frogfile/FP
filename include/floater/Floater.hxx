#pragma once

#include <cstdint>
#include <type_traits>

namespace Floaters
{
  class IFloater
  {
  public:
    IFloater() = default;

    IFloater(uint32_t &raw) : _raw(raw) {}

    IFloater(IFloater &&) = default;
    IFloater(const IFloater &) = default;
    IFloater &operator=(IFloater &&) = default;
    IFloater &operator=(const IFloater &) = default;
    ~IFloater() = default;

  private:
    uint32_t _raw{0x00'00'00'00};

    template <uint8_t DecBits>
    friend class Fixed;
  };
} // namespace Floater
