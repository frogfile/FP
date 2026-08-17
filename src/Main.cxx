#include "fp/FP.hxx"
#include "fp/Fixed.hxx"
#include <array>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <type_traits>

int main(int argc, char const *argv[]) {

  auto fixed4 =
      FP::Fixed<4, 2 * 8>(std::array<uint8_t, 4>{0x00, 16, 0x80, 0x00}); // 16.5

  auto fixed8 = FP::Fixed<8, 3 * 8>();

  std::cout << fixed4 << '\n';

  std::cout << fixed8 + fixed4;

  // std::cout << fixed4.relaxed<6, 3 * 8 + 2>() << '\n'; // expected 5 24

  return 0;
}
