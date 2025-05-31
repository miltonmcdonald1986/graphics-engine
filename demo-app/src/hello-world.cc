// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <iostream>

#include "graphics-engine/engine.h"
#include "graphics-engine/version.h"

using ::graphics_engine::engine::InitializeEngine;
using ::graphics_engine::types::Expected;
using ::graphics_engine::version::GetEngineLibVersion;

auto main() -> int {
  std::cout << "engine-lib:\n";
  std::cout << "  version: " << GetEngineLibVersion() << '\n';

  if (Expected<void> result = InitializeEngine(); !result.has_value()) {
    std::cerr << result.error().category().name() << ": "
              << result.error().message() << '\n';

    return result.error().value();
  }

  return 0;
}
