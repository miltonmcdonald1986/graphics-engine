// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/gl-wrappers.h"

#include <cassert>
#include <iostream>
#include <unordered_map>

#include "error.h"
#include "glad/glad.h"
#include "graphics-engine/gl-types.h"
#include "graphics-engine/types.h"

using enum graphics_engine::types::ErrorCode;
using enum graphics_engine::gl_types::GLBufferTarget;
using enum graphics_engine::gl_types::GLClearBit;
using enum graphics_engine::gl_types::GLDataType;
using enum graphics_engine::gl_types::GLDataUsagePattern;
using enum graphics_engine::gl_types::GLDrawMode;
using enum graphics_engine::gl_types::GLShaderType;

using graphics_engine::error::MakeErrorCode;
using graphics_engine::gl_types::GLBufferTarget;
using graphics_engine::gl_types::GLDataType;
using graphics_engine::gl_types::GLDataUsagePattern;
using graphics_engine::gl_types::GLDrawMode;
using graphics_engine::gl_types::GLShaderType;
using graphics_engine::i_gl_clear_flags::IGLClearFlags;
using graphics_engine::types::Expected;

using std::cerr;
using std::is_same_v;
using std::to_underlying;
using std::unexpected;

static_assert(is_same_v<GLbitfield, unsigned int>,
              "GLbitfield and unsigned int are not the same type!");
static_assert(is_same_v<GLboolean, unsigned char>,
              "GLboolean and unsigned char are not the same type!");
static_assert(is_same_v<GLint, int>, "GLint and int are not the same type!");
static_assert(is_same_v<GLsizei, int>,
              "GLsizei and int are not the same type!");

#ifdef _WIN64
static_assert(is_same_v<GLsizeiptr, long long int>,
              "GLsizeiptr and long long int are not the same type!");
#else
static_assert(is_same_v<GLsizeiptr, long int>,
              "GLsizeiptr and long long int are not the same type!");
#endif
static_assert(is_same_v<GLuint, unsigned int>,
              "GLuint and unsigned int are not the same type!");
static_assert(is_same_v<GLvoid, void>,
              "GLvoid and void are not the same type!");

namespace graphics_engine::gl_wrappers {

namespace {

auto ConvertGLBufferTarget(GLBufferTarget target) -> GLenum {
  switch (target) {
    default:
      assert(false);  // If we get here, add a new case to the switch.
      [[fallthrough]];
    case kArray:
      return GL_ARRAY_BUFFER;
    case kCopyRead:
      return GL_COPY_READ_BUFFER;
    case kCopyWrite:
      return GL_COPY_WRITE_BUFFER;
    case kElementArray:
      return GL_ELEMENT_ARRAY_BUFFER;
    case kPixelPack:
      return GL_PIXEL_PACK_BUFFER;
    case kPixelUnpack:
      return GL_PIXEL_UNPACK_BUFFER;
    case kTexture:
      return GL_TEXTURE;
    case kTransformFeedback:
      return GL_TRANSFORM_FEEDBACK_BUFFER;
    case kUniform:
      return GL_UNIFORM_BUFFER;
  }
}

auto ConvertGLDataType(GLDataType type) -> GLenum {
  switch (type) {
    default:
      assert(false);  // If we get here, add a new case to the switch.
      [[fallthrough]];
    case kByte:
      return GL_BYTE;
    case kUnsignedByte:
      return GL_UNSIGNED_BYTE;
    case kShort:
      return GL_SHORT;
    case kUnsignedShort:
      return GL_UNSIGNED_SHORT;
    case kInt:
      return GL_INT;
    case kUnsignedInt:
      return GL_UNSIGNED_INT;
    case kHalfFloat:
      return GL_HALF_FLOAT;
    case kFloat:
      return GL_FLOAT;
    case kDouble:
      return GL_DOUBLE;
    case kInt_2_10_10_10_Rev:
      return GL_INT_2_10_10_10_REV;
    case kUnsignedInt_2_10_10_10_Rev:
      return GL_UNSIGNED_INT_2_10_10_10_REV;
  }
}

auto ConvertGLDataUsagePattern(GLDataUsagePattern usage) -> GLenum {
  switch (usage) {
    default:
      assert(false);  // If we get here, add a new case to the switch.
      [[fallthrough]];
    case kStreamDraw:
      return GL_STREAM_DRAW;
    case kStreamRead:
      return GL_STREAM_READ;
    case kStreamCopy:
      return GL_STREAM_COPY;
    case kStaticDraw:
      return GL_STATIC_DRAW;
    case kStaticRead:
      return GL_STATIC_READ;
    case kStaticCopy:
      return GL_STATIC_COPY;
    case kDynamicDraw:
      return GL_DYNAMIC_DRAW;
    case kDynamicRead:
      return GL_DYNAMIC_READ;
    case kDynamicCopy:
      return GL_DYNAMIC_COPY;
  }
}

auto ConvertGLDrawMode(GLDrawMode mode) -> GLenum {
  switch (mode) {
    default:
      assert(false);  // If we get here, add a new case to the switch.
      [[fallthrough]];
    case kPoints:
      return GL_POINTS;
    case kLineStrip:
      return GL_LINE_STRIP;
    case kLineLoop:
      return GL_LINE_LOOP;
    case kLines:
      return GL_LINES;
    case kLineStripAdjacency:
      return GL_LINE_STRIP_ADJACENCY;
    case kLinesAdjacency:
      return GL_LINES_ADJACENCY;
    case kTriangleStrip:
      return GL_TRIANGLE_STRIP;
    case kTriangleFan:
      return GL_TRIANGLE_FAN;
    case kTriangles:
      return GL_TRIANGLES;
    case kTriangleStripAdjacency:
      return GL_TRIANGLE_STRIP_ADJACENCY;
    case kTrianglesAdjacency:
      return GL_TRIANGLES_ADJACENCY;
  }
}

auto ConvertGLShaderType(GLShaderType shader_type) -> GLenum {
  switch (shader_type) {
    default:
      std::cerr << "ConvertGLShaderType failed with underlying value "
                << static_cast<int>(shader_type) << '\n';
      assert(false);  // If we get here, add a new case to the switch.
      [[fallthrough]];
    case kFragment:
      return GL_FRAGMENT_SHADER;
    case kGeometry:
      return GL_GEOMETRY_SHADER;
    case kVertex:
      return GL_VERTEX_SHADER;
  }
}

}  // namespace

auto AttachShader(unsigned int program, unsigned int shader) -> Expected<void> {
  glAttachShader(program, shader);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glAttachShader failed with error code " << error << '\n';
    switch (error) {
      default:
        assert(false);  // If we get here, add a new case to the switch.
        [[fallthrough]];
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
    }
  }

  return {};
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
    cerr << "glBufferData failed with error code " << error << '\n';
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

auto Clear(const IGLClearFlags& flags) -> types::Expected<void> {
  GLbitfield mask = 0;
  if (flags.Test(kColor)) {
    mask |= GL_COLOR_BUFFER_BIT;
  }
  if (flags.Test(kDepth)) {
    mask |= GL_DEPTH_BUFFER_BIT;
  }
  if (flags.Test(kStencil)) {
    mask |= GL_STENCIL_BUFFER_BIT;
  }
  glClear(mask);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glClear failed with error code " << error << '\n';
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

auto CreateShader(GLShaderType shader_type) -> Expected<unsigned int> {
  GLenum gl_shader_type = ConvertGLShaderType(shader_type);
  GLuint shader = glCreateShader(gl_shader_type);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glCreateShader failed with error code " << error << '\n';
    switch (error) {
      default:
        assert(false);  // If we get here, add a new case to the switch.
        [[fallthrough]];
      case GL_INVALID_ENUM:
        return unexpected(MakeErrorCode(kGLErrorInvalidEnum));
    }
  }

  assert(shader > 0U);
  return shader;
}

auto DrawArrays(GLDrawMode mode, int first, int count) -> Expected<void> {
  GLenum gl_mode = ConvertGLDrawMode(mode);
  glDrawArrays(gl_mode, first, count);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glDrawArrays failed with error code " << error << '\n';
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
    }
  }

  return {};
}

auto EnableVertexAttribArray(unsigned int index) -> Expected<void> {
  glEnableVertexAttribArray(index);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glEnableVertexAttribArray failed with error code " << error
         << '\n';
    switch (error) {
      default:
        assert(false);  // If we get here, add a new case to the switch.
        [[fallthrough]];
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
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

auto UseProgram(unsigned int program) -> Expected<void> {
  glUseProgram(program);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glUseProgram failed with error code " << error << '\n';
    switch (error) {
      default:
        assert(false);  // If we get here, add a new case to the switch.
        [[fallthrough]];
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
    }
  }

  return {};
}

auto VertexAttribPointer(unsigned int index, int size, GLDataType type,
                         unsigned char normalized, int stride,
                         const void* pointer) -> Expected<void> {
  GLenum gl_type = ConvertGLDataType(type);
  glVertexAttribPointer(index, size, gl_type, normalized, stride, pointer);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glVertexAttribPointer failed with error code " << error << '\n';
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
    }
  }

  return {};
}

}  // namespace graphics_engine::gl_wrappers
