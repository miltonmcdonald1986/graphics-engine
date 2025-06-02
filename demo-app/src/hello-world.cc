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
  int error = glfwGetError(nullptr);
  assert(error == GLFW_NO_ERROR);

  std::cout << "engine-lib:\n";
  std::cout << "  version: " << GetEngineLibVersion() << '\n';

  Expected<void> result = InitializeEngine();
  assert(result.has_value());

  while (!glfwWindowShouldClose(window)) {
    error = glfwGetError(nullptr);
    assert(error == GLFW_NO_ERROR);

    // ClearColorBuffer();

    glfwSwapBuffers(window);
    error = glfwGetError(nullptr);
    assert(error == GLFW_NO_ERROR);

    glfwPollEvents();
    error = glfwGetError(nullptr);
    assert(error == GLFW_NO_ERROR);
  }

  glfwTerminate();
  error = glfwGetError(nullptr);
  assert(error == GLFW_NO_ERROR);

  return 0;
}
