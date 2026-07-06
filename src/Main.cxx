#include "floater/Fixed.hxx"

#include <iostream>
#include <bitset>

int main(int argc, char const *argv[])
{
  auto fixed = Floaters::Fixed<17>(0x00'00'00'0F, 0xC0'00'00'00);

  std::cout << fixed << '\n';

  std::cout << (double)fixed << '\n';

  return 0;
}
