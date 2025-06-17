// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_HELLO_TRIANGLE_H_
#define ENGINE_LIB_HELLO_TRIANGLE_H_

#include "graphics-engine/dll-export.h"
#include "graphics-engine/scene.h"
#include "graphics-engine/types.h"

namespace graphics_engine::hello_triangle {

class HelloTriangle : public scene::Scene {
 public:
  [[nodiscard]] auto Initialize() -> types::Expected<void>;
  [[nodiscard]] auto Render() const -> types::Expected<void> override;

 private:
  unsigned int shader_program_ = 0;
  unsigned int vao_ = 0;
};

using HelloTrianglePtr = std::unique_ptr<HelloTriangle>;
DLLEXPORT auto CreateHelloTriangleScene() -> HelloTrianglePtr;

}  // namespace graphics_engine::hello_triangle

#endif  // ENGINE_LIB_HELLO_TRIANGLE_H_
