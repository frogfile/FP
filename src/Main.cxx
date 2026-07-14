#include "fp/Fixed.hxx"
#include <array>
#include <bitset>
#include <cstdint>
#include <iostream>

int main(int argc, char const *argv[]) {
  auto fixed = FP::Fixed<4, 16>(std::array<uint8_t, 4>{0x00, 16, 0x80, 0x00});

  std::cout << (double)fixed;

  return 0;
}
