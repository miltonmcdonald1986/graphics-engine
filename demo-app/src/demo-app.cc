// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <cassert>
#include <iostream>

#include "GLFW/glfw3.h"
#include "graphics-engine/engine.h"
#include "graphics-engine/hello-triangle.h"
#include "graphics-engine/image.h"
#include "graphics-engine/version.h"

using ::glm::vec4;

using ::graphics_engine::engine::ClearBuffers;
using ::graphics_engine::engine::InitializeEngine;
using ::graphics_engine::engine::Render;
using ::graphics_engine::engine::SetBackgroundColor;
using ::graphics_engine::hello_triangle::HelloTriangle;
using ::graphics_engine::image::CaptureScreenshot;
using ::graphics_engine::types::Expected;
using ::graphics_engine::version::GetEngineLibVersion;

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
  
  HelloTriangle scene;
  auto expected_result = scene.Initialize();
  assert(expected_result.has_value());

  const vec4 defaultBackgroundColor{0.2F, 0.3F, 0.3F, 1.0F};
  SetBackgroundColor(defaultBackgroundColor);

  while (glfwWindowShouldClose(window) == GLFW_FALSE) {
    assert(glfwGetError(nullptr) == GLFW_NO_ERROR);

    ClearBuffers()
        .and_then([&scene]() { return scene.Render(); })
        .transform_error([](const std::error_code& err) 
          {
          std::cerr << err.message() << '\n';
          return err;
          });
    
    Expected<void> render_result = scene.Render();
    if (!render_result.has_value()) {
      break;
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
