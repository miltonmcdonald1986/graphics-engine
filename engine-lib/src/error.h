// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_ERROR_H_
#define ENGINE_LIB_ERROR_H_

#include <cassert>
#include <cstdint>
#include <system_error>

#include "glad/glad.h"
#include "graphics-engine/types.h"

namespace graphics_engine::error {

auto CheckGLError() -> void;
auto MakeErrorCode(::graphics_engine::types::ErrorCode code) -> std::error_code;

}  // namespace graphics_engine::error

#endif  // ENGINE_LIB_ERROR_H_
