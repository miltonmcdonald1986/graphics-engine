// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "error.h"

#include <utility>

using ::graphics_engine::types::ErrorCode;
using enum ::graphics_engine::types::ErrorCode;

using std::error_category;
using std::error_code;
using std::string;
using std::to_underlying;

namespace graphics_engine::error {

class ErrorCategory : public error_category {
 public:
  [[nodiscard]] auto name() const noexcept -> const char* override {
    return "graphics_engine::error";
  }

  [[nodiscard]] auto message(int condition) const -> string override {
    constexpr int expectedCount = 13;
    static_assert(to_underlying(kNumErrorCodes) == expectedCount,
                  "Update the switch statement below!");

    switch (static_cast<ErrorCode>(condition)) {
      default:
        assert(false);  // If we get here, add a new case to the switch
        [[fallthrough]];
      case kGladLoadGL:
        return "glad failed to load OpenGL.";
      case kGLError:
        return "OpenGL Error";
      case kGLErrorInvalidOperation:
        return "OpenGL Error: Invalid Operation.";
      case kGLErrorInvalidValue:
        return "OpenGL Error: Invalid Value.";
      case kGLErrorOutOfMemory:
        return "OpenGL Error: Out of Memory.";
      case kInvalidInput:
        return "Invalid input.";
      case kShaderError:
        return "Shader Error.";
      case kStbErrorLoad:
        return "Stb Error: Failed to load file.";
      case kStbErrorWritePng:
        return "Stb Error: Failed to write png file.";
    }
  }
};

auto GetErrorCategory() -> const ErrorCategory& {
  static ErrorCategory instance;
  return instance;
}

auto CheckGLError() -> void { assert(glGetError() == GL_NO_ERROR); }

auto MakeErrorCode(ErrorCode code) -> error_code {
  return {to_underlying(code), GetErrorCategory()};
}

}  // namespace graphics_engine::error
