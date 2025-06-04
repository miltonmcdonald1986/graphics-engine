// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_ENGINE_H_
#define ENGINE_LIB_ENGINE_H_

#include <filesystem>

#include "dll-export.h"
#include "glm/vec4.hpp"
#include "types.h"

namespace graphics_engine::engine {

DLLEXPORT [[nodiscard]] auto AreIdentical(const std::filesystem::path& png0,
                                          const std::filesystem::path& png1)
    -> ::graphics_engine::types::Expected<bool>;

/// @brief Capture a screenshot of the current rendering context.
/// @return path to screenshot on success, error on failure.
DLLEXPORT [[nodiscard]] auto CaptureScreenshot()
    -> ::graphics_engine::types::Expected<std::filesystem::path>;

/// @brief Initialize the graphics engine.
/// @return void on success, error on failure.
DLLEXPORT [[nodiscard]] auto InitializeEngine()
    -> ::graphics_engine::types::Expected<void>;

/// @brief Render everything in the graphics engine.
DLLEXPORT [[nodiscard]] auto Render()
    -> ::graphics_engine::types::Expected<void>;

/// @brief Sets the background color for the rendering engine.
/// @param color The new background color as a `glm::vec4` (RGBA format).
DLLEXPORT auto SetBackgroundColor(const glm::vec4& color) -> void;

}  // namespace graphics_engine::engine

#endif  // ENGINE_LIB_ENGINE_H_
