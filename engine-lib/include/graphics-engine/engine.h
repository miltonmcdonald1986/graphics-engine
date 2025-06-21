// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_ENGINE_H_
#define ENGINE_LIB_ENGINE_H_

#include "dll-export.h"
#include "glm/vec4.hpp"
#include "types.h"

namespace graphics_engine::engine {

/// @brief Initialize the graphics engine.
/// @return void on success, error on failure.
DLLEXPORT [[nodiscard]] auto InitializeEngine() -> types::Expected<void>;

/// @brief Render everything in the graphics engine.
/// @return void on success, error on failure.
DLLEXPORT [[nodiscard]] auto Render() -> types::Expected<void>;

/// @brief Sets the background color for the rendering engine.
/// @param color The new background color as a `glm::vec4` (RGBA format).
DLLEXPORT auto SetBackgroundColor(const glm::vec4& color) -> void;

}  // namespace graphics_engine::engine

#endif  // ENGINE_LIB_ENGINE_H_
