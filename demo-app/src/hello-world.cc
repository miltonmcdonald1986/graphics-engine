// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <iostream>

#include "graphics-engine/version.h"

using ::graphics_engine::version::GetEngineLibVersion;

auto main() -> int {
  std::cout << "engine-lib:\n";
  std::cout << "  version: " << GetEngineLibVersion() << '\n';
  return 0;
}
