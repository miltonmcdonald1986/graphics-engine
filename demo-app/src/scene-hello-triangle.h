// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef DEMO_APP_SCENE_HELLO_TRIANGLE_H_
#define DEMO_APP_SCENE_HELLO_TRIANGLE_H_

#include "graphics-engine/i-shader.h"
#include "graphics-engine/scene.h"
#include "graphics-engine/types.h"

namespace demo_app::scene_hello_triangle {

class HelloTriangle;
using HelloTrianglePtr = std::unique_ptr<HelloTriangle>;

class HelloTriangle : public graphics_engine::scene::Scene {
 public:
  ~HelloTriangle() override = default;

  [[nodiscard]] auto Render() const
      -> graphics_engine::types::Expected<void> override;

  friend auto CreateHelloTriangleScene() -> HelloTrianglePtr;

 private:
  HelloTriangle() = default;
  [[nodiscard]] auto Initialize() -> graphics_engine::types::Expected<void>;

  graphics_engine::shader::IShaderPtr shader_;
  unsigned int vao_ = 0;
};

auto CreateHelloTriangleScene() -> HelloTrianglePtr;

}  // namespace demo_app::scene_hello_triangle

#endif  // DEMO_APP_SCENE_HELLO_TRIANGLE_H_
