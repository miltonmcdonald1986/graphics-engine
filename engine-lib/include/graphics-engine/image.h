// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_IMAGE_H_
#define ENGINE_LIB_IMAGE_H_

#include <filesystem>
#include <optional>

#include "dll-export.h"
#include "types.h"

namespace graphics_engine::image {

/// @brief Determine if two png files are identical.
/// @param png0 The first png file to compare.
/// @param png1 The second png file to compare.
/// @return true if the files are identical, false if they differ, and error if
/// something went wrong while trying to compare them.
DLLEXPORT [[nodiscard]] auto AreIdentical(const ::std::filesystem::path& png0,
                                          const ::std::filesystem::path& png1)
    -> ::graphics_engine::types::Expected<bool>;

/// @brief Capture a screenshot of the current rendering context.
/// @param dest Optional destination path for the screenshot.
/// @return path to screenshot on success, error on failure.
/// @note If `dest` is not provided, the screenshot will be saved to
/// std::filesystem::temp_directory_path()/"screenshot.png"
DLLEXPORT [[nodiscard]] auto CaptureScreenshot(
    const ::std::optional<::std::filesystem::path>& dest = ::std::nullopt)
    -> ::graphics_engine::types::Expected<void>;

}  // namespace graphics_engine::image

#endif  // ENGINE_LIB_IMAGE_H_
