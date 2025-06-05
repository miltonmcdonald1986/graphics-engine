// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "error.h"

#include <utility>

using enum ::graphics_engine::error::ErrorCode;

namespace graphics_engine::error {

class ErrorCategory : public std::error_category {
 public:
  [[nodiscard]] auto name() const noexcept -> const char* override {
    return "graphics_engine::error";
  }

  [[nodiscard]] auto message(int condition) const -> std::string override {
    constexpr int expectedCount = 8;
    static_assert(std::to_underlying(kCount) == expectedCount,
                  "Update the switch statement below!");

    switch (static_cast<ErrorCode>(condition)) {
      default:
      case kEngineInitializationFailed:
        return "Engine Initialization failed.";
      case kGLErrorInvalidEnum:
        return "OpenGL Error: Invalid Enum.";
      case kGLErrorInvalidOperation:
        return "OpenGL Error: Invalid Operation.";
      case kGLErrorInvalidValue:
        return "OpenGL Error: Invalid Value.";
      case kStbErrorLoad:
        return "Stb Error: Failed to load file.";
      case kStbErrorWritePng:
        return "Stb Error: Failed to write png file.";
      case kUnknownError:
        return "An unknown error occurred.";
    }
  }
};

auto GetErrorCategory() -> const ErrorCategory& {
  static ErrorCategory instance;
  return instance;
}

auto MakeErrorCode(ErrorCode code) -> std::error_code {
  return {std::to_underlying(code), GetErrorCategory()};
}

}  // namespace graphics_engine::error
