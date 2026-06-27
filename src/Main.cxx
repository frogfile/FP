#include "floater/Fixed.hxx"

#include <iostream>

int main(int argc, char const *argv[])
{
  auto fixed = Floaters::Fixed<16>(0x000F, 0xC000);

  std::cout << fixed << '\n';

  std::cout << (double)fixed << '\n';

  return 0;
}
