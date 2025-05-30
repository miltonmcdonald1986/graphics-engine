// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/version.h"

namespace {

constexpr int kEngineLibVersionMajor = 1;
constexpr int kEngineLibVersionMinor = 0;
constexpr int kEngineLinVersionPatch = 0;

constexpr std::string_view kEngineLibVersionString = "1.0.0";

}  // namespace

namespace graphics_engine::version {

[[nodiscard]] auto GetEngineLibVersion() -> std::string_view {
  return kEngineLibVersionString;
}

}  // namespace graphics_engine::version
