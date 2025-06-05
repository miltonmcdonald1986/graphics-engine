// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/engine.h"

#include "error.h"
#include "glad/glad.h"

using ::glm::vec4;

using enum ::graphics_engine::error::ErrorCode;
using ::graphics_engine::types::Expected;

using ::std::unexpected;

namespace graphics_engine::engine {

auto ClearBuffers() -> Expected<void> {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  switch (glGetError()) {
    case GL_INVALID_VALUE:
      return unexpected(MakeErrorCode(kGLErrorInvalidValue));
    case GL_NO_ERROR:
      return {};
    default:
      return unexpected(MakeErrorCode(kUnknownError));
  }
}

auto InitializeEngine() -> Expected<void> {
  if (gladLoadGL() == 0) {
    return unexpected(MakeErrorCode(kEngineInitializationFailed));
  }

  return {};
}

auto Render() -> Expected<void> {
  glClear(GL_COLOR_BUFFER_BIT);
  assert(glGetError() == GL_NO_ERROR);
  return {};
}

auto SetBackgroundColor(const vec4& color) -> void {
  glClearColor(color[0], color[1], color[2], color[3]);
}

}  // namespace graphics_engine::engine
