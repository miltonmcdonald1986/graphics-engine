// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/gl-wrappers.h"

#include <cassert>
#include <iostream>
#include <unordered_map>

#include "glad/glad.h"

#include "graphics-engine/types.h"

#include "error.h"

using enum ::graphics_engine::types::ErrorCode;
using enum graphics_engine::types::GLBufferTarget;

using ::graphics_engine::types::Expected;
using ::graphics_engine::types::GLBufferTarget;
using ::graphics_engine::error::MakeErrorCode;

using ::std::cerr;
using ::std::is_same_v;
using ::std::unexpected;

static_assert(is_same_v<GLsizei, int>, "GLint and int are not the same type!");
static_assert(is_same_v<GLuint, unsigned int>,
              "GLuint and unsigned int are not the same type!");

namespace graphics_engine::gl_wrappers {

auto BindBuffer(GLBufferTarget target, unsigned int buffer) -> Expected<void> {
  GLenum gl_target{};
  switch (target) {
    default:
      assert(false); // If we get here, add a new case to the switch.
      [[fallthrough]];
    case Array:
      gl_target = GL_ARRAY_BUFFER;
      break;
    case CopyRead:
      gl_target = GL_COPY_READ_BUFFER;
      break;
    case CopyWrite:
      gl_target = GL_COPY_WRITE_BUFFER;
      break;
    case ElementArray:
      gl_target = GL_ELEMENT_ARRAY_BUFFER;
      break;
    case PixelPack:
      gl_target = GL_PIXEL_PACK_BUFFER;
      break;
    case PixelUnpack:
      gl_target = GL_PIXEL_UNPACK_BUFFER;
      break;
    case Texture:
      gl_target = GL_TEXTURE;
      break;
    case TransformFeedback:
      gl_target = GL_TRANSFORM_FEEDBACK_BUFFER;
      break;
    case Uniform:
      gl_target = GL_UNIFORM_BUFFER;
      break;
  }

  glBindBuffer(gl_target, buffer);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glBindBuffer failed with error code " << error << '\n';
    switch (error) {
      default:
        assert(false);  // If we get here, add a new case to the switch.
        [[fallthrough]];
      case GL_INVALID_ENUM:
        return unexpected(MakeErrorCode(kGLErrorInvalidEnum));
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
    }
  }

  return {};
}

auto BindVertexArray(unsigned int array) -> Expected<void> {
  glBindVertexArray(array);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glBindVertexArray failed with error code " << error << '\n';
    switch (error) {
      default:
        assert(false);  // If we get here, add a new case to the switch.
        [[fallthrough]];
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
    }
  }

  return {};
}

auto GenBuffers(int n, unsigned int* buffers) -> Expected<void> {
  glGenBuffers(n, buffers);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glGenBuffers failed with error code " << error << '\n';
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
auto GenVertexArrays(int n, unsigned int* arrays) -> Expected<void> {
  glGenVertexArrays(n, arrays);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glGenVertexArrays failed with error code " << error << '\n';
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
