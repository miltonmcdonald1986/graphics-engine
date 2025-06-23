// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_TYPES_H_
#define ENGINE_LIB_TYPES_H_

#include <cstdint>
#include <expected>
#include <system_error>
#include <unordered_map>

#include "gl-types.h"

namespace graphics_engine::types {

enum class ErrorCode : std::uint8_t {
  // kNoError = 0,
  kGladLoadGL = 1,
  kGLError,
  kGLErrorInvalidEnum,
  kGLErrorInvalidOperation,
  kGLErrorInvalidValue,
  kGLErrorOutOfMemory,
  kInvalidShaderType,
  kSceneInitFailure,
  kShaderError,
  kStbErrorLoad,
  kStbErrorWritePng,
  kNumErrorCodes  // Sentinel value to track enum size
};

template <typename T>
using Expected = std::expected<T, std::error_code>;

using ShaderSourceMap = std::unordered_map<gl_types::GLShaderType, std::string>;

}  // namespace graphics_engine::types

#endif  // ENGINE_LIB_TYPES_H_
