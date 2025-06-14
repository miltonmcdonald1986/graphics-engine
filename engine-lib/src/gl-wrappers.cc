// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/gl-wrappers.h"

#include <cassert>
#include <iostream>
#include <unordered_map>

#include "error.h"
#include "glad/glad.h"
#include "graphics-engine/types.h"

using enum ::graphics_engine::types::ErrorCode;
using enum graphics_engine::types::GLBufferTarget;
using enum ::graphics_engine::types::GLDataUsagePattern;

using ::graphics_engine::error::MakeErrorCode;
using ::graphics_engine::types::Expected;
using ::graphics_engine::types::GLBufferTarget;
using ::graphics_engine::types::GLDataUsagePattern;

using ::std::cerr;
using ::std::is_same_v;
using ::std::unexpected;

static_assert(is_same_v<GLsizei, int>, "GLint and int are not the same type!");
static_assert(is_same_v<GLsizeiptr, long long int>,
              "GLsizeiptr and long long int are not the same type!");
static_assert(is_same_v<GLuint, unsigned int>,
              "GLuint and unsigned int are not the same type!");
static_assert(is_same_v<GLvoid, void>,
              "GLvoid and void are not the same type!");

namespace graphics_engine::gl_wrappers {

namespace {

auto ConvertGLBufferTarget(GLBufferTarget target) -> GLenum 
{
  switch (target) {
    default:
      assert(false);  // If we get here, add a new case to the switch.
      [[fallthrough]];
    case Array:
      return GL_ARRAY_BUFFER;
    case CopyRead:
      return GL_COPY_READ_BUFFER;
    case CopyWrite:
      return GL_COPY_WRITE_BUFFER;
    case ElementArray:
      return GL_ELEMENT_ARRAY_BUFFER;
    case PixelPack:
      return GL_PIXEL_PACK_BUFFER;
    case PixelUnpack:
      return GL_PIXEL_UNPACK_BUFFER;
    case Texture:
      return GL_TEXTURE;
    case TransformFeedback:
      return GL_TRANSFORM_FEEDBACK_BUFFER;
    case Uniform:
      return GL_UNIFORM_BUFFER;
  }
}

auto ConvertGLDataUsagePattern(GLDataUsagePattern usage) -> GLenum {
  switch (usage) {
    default:
      assert(false); // If we get here, add a new case to the switch.
      [[fallthrough]];
    case StreamDraw:
      return GL_STREAM_DRAW;
    case StreamRead:
      return GL_STREAM_READ;
    case StreamCopy:
      return GL_STREAM_COPY;
    case StaticDraw:
      return GL_STATIC_DRAW;
    case StaticRead:
      return GL_STATIC_READ;
    case StaticCopy:
      return GL_STATIC_COPY;
    case DynamicDraw:
      return GL_DYNAMIC_DRAW;
    case DynamicRead:
      return GL_DYNAMIC_READ;
    case DynamicCopy:
      return GL_DYNAMIC_COPY;
  }
}

}

auto BindBuffer(GLBufferTarget target, unsigned int buffer) -> Expected<void> {
  GLenum gl_target = ConvertGLBufferTarget(target);
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

auto BufferData(GLBufferTarget target, long long int size, const void* data,
                GLDataUsagePattern usage) -> Expected<void> {
  GLenum gl_target = ConvertGLBufferTarget(target);
  GLenum gl_usage = ConvertGLDataUsagePattern(usage);
  glBufferData(gl_target, size, data, gl_usage);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glGenBuffers failed with error code " << error << '\n';
    switch (error) {
      default:
        assert(false);  // If we get here, add a new case to the switch.
        [[fallthrough]];
      case GL_INVALID_ENUM:
        return unexpected(MakeErrorCode(kGLErrorInvalidEnum));
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
      case GL_OUT_OF_MEMORY:
        return unexpected(MakeErrorCode(kGLErrorOutOfMemory));
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
