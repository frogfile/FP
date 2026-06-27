#pragma once

#include <sstream>
#include <cstdint>
#include <bitset>
#include <cmath>

#include "Floater.hxx"

namespace Floaters
{
    template <uint8_t DecBits>
    class Fixed : IFloater
    {
    public:
        Fixed(double &val);

        Fixed(uint16_t integer, uint16_t raw_decimal)
        {
            _raw = (static_cast<uint32_t>(integer) << NumDecimalBits()) + raw_decimal;
        }

        constexpr uint32_t Raw() const
        {
            return _raw;
        };

        constexpr uint16_t IntegerPart() const
        {
            return static_cast<uint16_t>(_raw >> NumDecimalBits());
        }

        constexpr uint16_t DecimalPartRaw() const
        {
            // NOLINTNEXTLINE
            return static_cast<uint16_t>(_raw & 0x0000'FFFF);
        }

        constexpr double DecPartAsDouble() const
        {
            uint16_t dec = DecimalPartRaw();
            double acc = 0.0F;

            for (uint8_t i = DecBits; i > 0; i--)
            {
                acc += (dec & 1) * (1 / std::pow(2, i));
                dec >>= 1;
            }
            return acc;
        }

        constexpr operator double() const
        {
            uint32_t buf = _raw;
            double acc = (double)IntegerPart();

            for (uint8_t i = DecBits; i > 0; i--)
            {
                acc += (buf & 1) * (1 / std::pow(2, i));
                buf >>= 1;
            }
            return acc;
        }

        friend std::ostream &
        operator<<(std::ostream &out, Fixed const &fixed)
        {
            // NOLINTNEXTLINE
            out << std::bitset<(sizeof(Fixed::_raw) * 8) - DecBits>(fixed.IntegerPart())
                << '.'
                << std::bitset<DecBits>(fixed.DecimalPartRaw());
            return out;
        };

    private:
        // NOLINTNEXTLINE
        static constexpr uint8_t NumDecimalBits() { return DecBits; };

        static constexpr uint8_t NumIntegerBits()
        {
            // NOLINTNEXTLINE
            return static_cast<uint8_t>(sizeof(Fixed::_raw) * 8) - DecBits;
        }
    };
} // namespace Floater
