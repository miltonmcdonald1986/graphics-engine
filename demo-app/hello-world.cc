#include <iostream>

auto DoTheThing() -> int 
{
  return 1; 
}

auto main() -> int {
  int unusedVar;
  std::cout << "Hello, world!\n";
  return 0;
}
