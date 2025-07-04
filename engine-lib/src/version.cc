// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/version.h"

#include <format>

using std::format;
using std::string;

namespace {

constexpr int kEngineLibVersionMajor = 1;
constexpr int kEngineLibVersionMinor = 0;
constexpr int kEngineLinVersionPatch = 0;

}  // namespace

namespace graphics_engine::version {

[[nodiscard]] auto GetEngineLibVersion() -> string {
  return format("{}.{}.{}", kEngineLibVersionMajor, kEngineLibVersionMinor,
                kEngineLinVersionPatch);
}

}  // namespace graphics_engine::version
