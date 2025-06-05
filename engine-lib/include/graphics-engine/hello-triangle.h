// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_HELLO_TRIANGLE_H_
#define ENGINE_LIB_HELLO_TRIANGLE_H_

#include "graphics-engine/dll-export.h"
#include "graphics-engine/types.h"

namespace graphics_engine::hello_triangle {

class DLLEXPORT HelloTriangle {
 public:
  auto [[nodiscard]] Initialize() -> ::graphics_engine::types::Expected<void>;
  auto [[nodiscard]] Render() -> ::graphics_engine::types::Expected<void>;

 private:
  unsigned int shader_program_{};
  unsigned int vao_{};
};

}  // namespace graphics_engine::hello_triangle

#endif  // ENGINE_LIB_HELLO_TRIANGLE_H_
