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

        /**
         * @brief Construct a new Fixed object from raw integer and decimal bits
         * the representation is anchored around the decimal point and in case
         * any/both parts don't fit, the integer will be cut off from the left
         * and decimal from the right.
         *
         * @param raw_integer regular unsigned int bits
         * @param raw_decimal decimal bits
         */
        Fixed(uint32_t raw_integer, uint32_t raw_decimal)
        {
            // FIXME: Decimal gets cut off on the left for some reason
            _raw = (raw_integer << NumDecimalBits()) + (raw_decimal >> NumIntegerBits());
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
            // FIXME: Hardcoded 16 bit
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
            return (double)IntegerPart() + DecPartAsDouble();
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
        static constexpr uint8_t NumDecimalBits() { return DecBits; }

        static constexpr uint8_t NumIntegerBits()
        {
            // NOLINTNEXTLINE
            return static_cast<uint8_t>(sizeof(Fixed::_raw) * 8) - DecBits;
        }
    };
} // namespace Floater
