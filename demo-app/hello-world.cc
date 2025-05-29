#include <iostream>

#include "graphics-engine/version.h"

using ::graphics_engine::GetEngineLibVersion;

auto DoTheThing() -> int { return 1; }

auto main() -> int {
  std::cout << "engine-lib:\n";
  std::cout << "  version: " << GetEngineLibVersion() << '\n';
  return 0;
}
