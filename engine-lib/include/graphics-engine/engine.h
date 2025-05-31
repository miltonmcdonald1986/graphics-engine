// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_ENGINE_H_
#define ENGINE_LIB_ENGINE_H_

#include "dll-export.h"
#include "types.h"

namespace graphics_engine::engine {

/// @brief Initialize the graphics engine.
/// @return void on success, error on failure.
DLLEXPORT [[nodiscard]] auto InitializeEngine()
    -> ::graphics_engine::types::Expected<void>;

}  // namespace graphics_engine::engine

#endif  // ENGINE_LIB_ENGINE_H_
