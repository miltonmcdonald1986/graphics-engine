// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/engine.h"

#include "glad/glad.h"

#include "error.h"

using ::graphics_engine::error::ErrorCode;
using ::graphics_engine::types::Expected;

namespace graphics_engine::engine {

DLLEXPORT [[nodiscard]] auto InitializeEngine() -> Expected<void> 
{
  if (gladLoadGL() == 0)
    return std::unexpected(make_error_code(ErrorCode::kEngineInitializationFailed));

  return {};
}

}  // namespace graphics_engine::engine
