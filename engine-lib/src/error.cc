// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "error.h"

#include <utility>

namespace graphics_engine::error {

class ErrorCategory : public std::error_category {
 public:
  [[nodiscard]] auto name() const noexcept -> const char* override {
    return "graphics_engine::error";
  }

  [[nodiscard]] auto message(int ev) const -> std::string override {
    static_assert(std::to_underlying(ErrorCode::kCount) == 2,
                  "Update the switch statement below!");

    switch (static_cast<ErrorCode>(ev)) {
      default:
      case ErrorCode::kEngineInitializationFailed:
        return "Engine Initialization failed.";
    }
  }
};

auto GetErrorCategory() -> const ErrorCategory& {
  static ErrorCategory instance;
  return instance;
}

auto make_error_code(graphics_engine::error::ErrorCode e) -> std::error_code {
  return {std::to_underlying(e), graphics_engine::error::GetErrorCategory()};
}

}  // namespace graphics_engine::error
