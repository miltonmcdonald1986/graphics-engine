// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/engine.h"

#include "error.h"

using ::graphics_engine::error::ErrorCode;
using ::graphics_engine::types::Expected;

namespace graphics_engine::engine {

DLLEXPORT [[nodiscard]] auto InitializeEngine() -> Expected<void> { return {}; }

}  // namespace graphics_engine::engine
