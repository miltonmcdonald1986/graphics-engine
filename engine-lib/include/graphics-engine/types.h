// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_TYPES_H_
#define ENGINE_LIB_TYPES_H_

#include <expected>
#include <system_error>

namespace graphics_engine::types {

template <typename T>
using Expected = std::expected<T, std::error_code>;

}  // namespace graphics_engine::types

#endif  // ENGINE_LIB_TYPES_H_
