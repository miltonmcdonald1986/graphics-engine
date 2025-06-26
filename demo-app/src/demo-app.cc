// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <cassert>
#include <iostream>
#include <utility>

#include "GLFW/glfw3.h"
#include "graphics-engine/engine.h"
#include "graphics-engine/gl-wrappers.h"
#include "graphics-engine/image.h"
#include "graphics-engine/version.h"
#include "scene-hello-triangle.h"

using demo_app::scene_hello_triangle::CreateHelloTriangleScene;
using demo_app::scene_hello_triangle::HelloTrianglePtr;

using glm::vec4;

using enum graphics_engine::gl_types::GLClearBit;

using graphics_engine::engine::InitializeEngine;
using graphics_engine::engine::Render;
using graphics_engine::engine::SetBackgroundColor;
using graphics_engine::gl_clear_flags::CreateIGLClearFlags;
using graphics_engine::gl_clear_flags::IGLClearFlagsPtr;
using graphics_engine::gl_wrappers::Clear;
using graphics_engine::image::CaptureScreenshot;
using graphics_engine::types::Expected;
using graphics_engine::version::GetEngineLibVersion;

using std::cerr;
using std::error_code;
using std::to_underlying;

auto main() -> int {
#ifdef _WIN32
  // Enable Memory Leak Detection
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  int res_init = glfwInit();
  if (res_init == GLFW_FALSE) {
    assert(false);
    return -1;
  }

  const int width = 640;
  const int height = 480;
  const std::string title = "Hello world!";
  GLFWwindow* window =
      glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  assert(window != nullptr);

  glfwMakeContextCurrent(window);
  assert(glfwGetError(nullptr) == GLFW_NO_ERROR);

  std::cout << "engine-lib:\n";
  std::cout << "  version: " << GetEngineLibVersion() << '\n';

  Expected<void> result = InitializeEngine();
  if (!result.has_value()) {
    assert(false);

    glfwTerminate();
    assert(glfwGetError(nullptr) == GLFW_NO_ERROR);

    return -1;
  }

  HelloTrianglePtr hello_triangle_scene = CreateHelloTriangleScene();
  if (hello_triangle_scene == nullptr) {
    assert(false);
    return -1;
  }

  const vec4 defaultBackgroundColor{0.2F, 0.3F, 0.3F, 1.0F};
  SetBackgroundColor(defaultBackgroundColor);

  IGLClearFlagsPtr flags{CreateIGLClearFlags()};
  flags->Set(kColor).Set(kDepth).Set(kStencil);

  while (glfwWindowShouldClose(window) == GLFW_FALSE) {
    assert(glfwGetError(nullptr) == GLFW_NO_ERROR);

    Expected<void> did_clear = Clear(*flags);
    if (!did_clear.has_value()) {
      const error_code& err = did_clear.error();
      cerr << err.message() << '\n';
      return err.value();
    }

    Expected<void> did_render = hello_triangle_scene->Render();
    if (!did_render.has_value()) {
      const error_code& err = did_render.error();
      cerr << err.message() << '\n';
      return err.value();
    }

    glfwSwapBuffers(window);
    assert(glfwGetError(nullptr) == GLFW_NO_ERROR);

    glfwPollEvents();
    assert(glfwGetError(nullptr) == GLFW_NO_ERROR);
  }

  glfwTerminate();
  assert(glfwGetError(nullptr) == GLFW_NO_ERROR);

  return 0;
}
