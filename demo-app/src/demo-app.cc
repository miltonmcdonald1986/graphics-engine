// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <cassert>
#include <iostream>

#include "GLFW/glfw3.h"
#include "graphics-engine/engine.h"
#include "graphics-engine/version.h"

using ::graphics_engine::engine::InitializeEngine;
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

  while (glfwWindowShouldClose(window) == GLFW_FALSE) {
    assert(glfwGetError(nullptr) == GLFW_NO_ERROR);

    // ClearColorBuffer();

    glfwSwapBuffers(window);
    assert(glfwGetError(nullptr) == GLFW_NO_ERROR);

    glfwPollEvents();
    assert(glfwGetError(nullptr) == GLFW_NO_ERROR);
  }

  glfwTerminate();
  assert(glfwGetError(nullptr) == GLFW_NO_ERROR);

  return 0;
}
