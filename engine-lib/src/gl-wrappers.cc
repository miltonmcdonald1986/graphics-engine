// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/gl-wrappers.h"

#include <cassert>
#include <iostream>

#include "glad/glad.h"

#include "graphics-engine/types.h"

#include "error.h"

using enum ::graphics_engine::types::ErrorCode;

using ::graphics_engine::error::MakeErrorCode;
using ::graphics_engine::types::Expected;

using ::std::cerr;
using ::std::is_same_v;
using ::std::unexpected;

static_assert(is_same_v<GLsizei, int>, "GLint and int are not the same type!");
static_assert(is_same_v<GLuint, unsigned int>,
              "GLuint and unsigned int are not the same type!");

namespace graphics_engine::gl_wrappers {

auto GenVertexArrays(int n, unsigned int* arrays) -> Expected<void> {
  glGenVertexArrays(n, arrays);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glAttachShader failed with error code " << error << '\n';
    switch (error) {
      default:
        assert(false);  // If we get here, add a new case to the switch.
        [[fallthrough]];
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
    }
  }

  return {};
}

}  // namespace graphics_engine::gl_wrappers
