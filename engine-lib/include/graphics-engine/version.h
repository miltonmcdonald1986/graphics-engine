// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_VERSION_H_
#define ENGINE_LIB_VERSION_H_

#include <string_view>

#include "dll-export.h"

namespace graphics_engine::version {

/**
 * @brief Retrieves the current version of the engine library.
 *
 * This function returns a string view representing the library version,
 * which can be used for debugging, logging, or version validation.
 *
 * @return A `std::string_view` containing the engine library version.
 */
DLLEXPORT [[nodiscard]] auto GetEngineLibVersion() -> std::string_view;

}  // namespace graphics_engine::version

#endif  // ENGINE_LIB_VERSION_H_
