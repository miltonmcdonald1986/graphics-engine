// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_ERROR_H_
#define ENGINE_LIB_ERROR_H_

#include <cstdint>
#include <system_error>

namespace graphics_engine::error {

enum class ErrorCode : std::uint8_t {
  // kNoError = 0,
  kEngineInitializationFailed = 1,
  kGLErrorInvalidEnum,
  kGLErrorInvalidOperation,
  kGLErrorInvalidValue,
  kStbErrorLoad,
  kStbErrorWritePng,
  kCount  // Sentinel value to track enum size
};

auto MakeErrorCode(ErrorCode code) -> std::error_code;

}  // namespace graphics_engine::error

#endif  // ENGINE_LIB_ERROR_H_
