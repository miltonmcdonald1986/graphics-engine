// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/gl-wrappers.h"

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <utility>

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
using enum graphics_engine::gl_types::GLShaderObjectParameter;
using enum graphics_engine::gl_types::GLShaderType;

using graphics_engine::error::MakeErrorCode;
using graphics_engine::gl_clear_flags::IGLClearFlags;
using graphics_engine::gl_types::GLBufferTarget;
using graphics_engine::gl_types::GLDataType;
using graphics_engine::gl_types::GLDataUsagePattern;
using graphics_engine::gl_types::GLDrawMode;
using graphics_engine::gl_types::GLShaderObjectParameter;
using graphics_engine::gl_types::GLShaderType;
using graphics_engine::types::Expected;

using std::cerr;
using std::is_same_v;
using std::to_underlying;
using std::unexpected;

static_assert(is_same_v<GLbitfield, unsigned int>,
              "GLbitfield and unsigned int are not the same type!");
static_assert(is_same_v<GLboolean, unsigned char>,
              "GLboolean and unsigned char are not the same type!");
static_assert(is_same_v<GLchar, char>,
              "GLchar and char are not the same type!");
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

auto ConvertGLBufferTarget(GLBufferTarget target) -> Expected<GLenum> {
  switch (target) {
    default:
      assert(false);  // If we get here, add a new case to the switch.
      return unexpected(MakeErrorCode(kInvalidInput));
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

auto ConvertGLDataType(GLDataType type) -> Expected<GLenum> {
  switch (type) {
    default:
      assert(false);  // If we get here, add a new case to the switch.
      return unexpected(MakeErrorCode(kInvalidInput));
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

auto ConvertGLDataUsagePattern(GLDataUsagePattern usage) -> Expected<GLenum> {
  switch (usage) {
    default:
      assert(false);  // If we get here, add a new case to the switch.
      return unexpected(MakeErrorCode(kInvalidInput));
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

auto ConvertGLDrawMode(GLDrawMode mode) -> Expected<GLenum> {
  switch (mode) {
    default:
      assert(false);  // If we get here, add a new case to the switch.
      return unexpected(MakeErrorCode(kInvalidInput));
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

auto ConvertGLShaderObjectParameter(GLShaderObjectParameter pname)
    -> Expected<GLenum> {
  switch (pname) {
    default:
      assert(false);  // If we get here, add a new case to the switch.
      return unexpected(MakeErrorCode(kInvalidInput));
    case kShaderType:
      return GL_SHADER_TYPE;
    case kDeleteStatus:
      return GL_DELETE_STATUS;
    case kCompileStatus:
      return GL_COMPILE_STATUS;
    case kInfoLogLength:
      return GL_INFO_LOG_LENGTH;
    case kShaderSourceLength:
      return GL_SHADER_SOURCE_LENGTH;
  }
}

auto ConvertGLShaderType(GLShaderType shader_type) -> Expected<GLenum> {
  switch (shader_type) {
    default:
      assert(false);  // If we get here, add a new case to the switch.
      return unexpected(MakeErrorCode(kInvalidInput));
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
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
    }
  }

  return {};
}

auto BindBuffer(GLBufferTarget target, unsigned int buffer) -> Expected<void> {
  Expected<GLenum> gl_target = ConvertGLBufferTarget(target);
  if (!gl_target) {
    cerr << "ConvertGLBufferTarget failed with error code " << gl_target.error()
         << ": " << gl_target.error().message() << '\n';
    return unexpected(gl_target.error());
  }

  glBindBuffer(*gl_target, buffer);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glBindBuffer failed with error code " << error << '\n';
    switch (error) {
      default:
        // LCOV_EXCL_START
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
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
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
    }
  }

  return {};
}

auto BufferData(GLBufferTarget target, long long int size, const void* data,
                GLDataUsagePattern usage) -> Expected<void> {
  Expected<GLenum> gl_target = ConvertGLBufferTarget(target);
  if (!gl_target) {
    cerr << "ConvertGLBufferTarget failed with error code " << gl_target.error()
         << ": " << gl_target.error().message() << '\n';
    return unexpected(gl_target.error());
  }

  Expected<GLenum> gl_usage = ConvertGLDataUsagePattern(usage);
  if (!gl_usage) {
    cerr << "ConvertGLDataUsagePattern failed with error code "
         << gl_usage.error() << ": " << gl_usage.error().message() << '\n';
    return unexpected(gl_usage.error());
  }

  glBufferData(*gl_target, size, data, *gl_usage);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glBufferData failed with error code " << error << '\n';
    switch (error) {
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
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
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
    }
  }

  return {};
}

auto CompileShader(unsigned int shader) -> Expected<void> {
  glCompileShader(shader);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glCompileShader failed with error code " << error << '\n';
    switch (error) {
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
    }
  }

  return {};
}

auto CreateProgram() -> Expected<unsigned int> {
  GLuint program_id = glCreateProgram();
  if (program_id == 0) {
    cerr << "An error occurred creating the program object.";
    return unexpected(MakeErrorCode(kGLError));
  }

  return program_id;
}

auto CreateShader(GLShaderType shader_type) -> Expected<unsigned int> {
  Expected<GLenum> gl_shader_type = ConvertGLShaderType(shader_type);
  if (!gl_shader_type) {
    cerr << "ConvertGLShaderType failed with error code "
         << gl_shader_type.error() << ": " << gl_shader_type.error().message()
         << '\n';
    return unexpected(gl_shader_type.error());
  }

  GLuint shader = glCreateShader(*gl_shader_type);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glCreateShader failed with error code " << error << '\n';
    switch (error) {
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
      case GL_INVALID_ENUM:
        return unexpected(MakeErrorCode(kGLErrorInvalidEnum));
    }
  }

  assert(shader > 0U);
  return shader;
}

auto DrawArrays(GLDrawMode mode, int first, int count) -> Expected<void> {
  Expected<GLenum> gl_mode = ConvertGLDrawMode(mode);
  if (!gl_mode) {
    cerr << "ConvertGLDrawMode failed with error code " << gl_mode.error()
         << ": " << gl_mode.error().message() << '\n';
    return unexpected(gl_mode.error());
  }

  glDrawArrays(*gl_mode, first, count);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glDrawArrays failed with error code " << error << '\n';
    switch (error) {
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
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
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
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
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
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
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
    }
  }

  return {};
}

DLLEXPORT [[nodiscard]] auto GetShaderInfoLog(unsigned int shader,
                                              int max_length, int* length,
                                              char* info_log)
    -> types::Expected<void> {
  glGetShaderInfoLog(shader, max_length, length, info_log);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glGetShaderInfoLog failed with error code " << error << '\n';
    switch (error) {
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
    }
  }

  return {};
}

DLLEXPORT [[nodiscard]] auto GetShaderiv(
    unsigned int shader, gl_types::GLShaderObjectParameter pname, int* params)
    -> types::Expected<void> {
  Expected<GLenum> gl_pname = ConvertGLShaderObjectParameter(pname);
  if (!gl_pname) {
    cerr << "ConvertGLShaderObjectParameter failed with error code "
         << gl_pname.error() << ": " << gl_pname.error().message() << '\n';
    return unexpected(gl_pname.error());
  }

  glGetShaderiv(shader, *gl_pname, params);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glGetShaderiv failed with error code " << error << '\n';
    switch (error) {
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
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

auto LinkProgram(unsigned int program) -> types::Expected<void> {
  glLinkProgram(program);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glLinkProgram failed with error code " << error << '\n';
    switch (error) {
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
    }
  }

  return {};
}

auto ShaderSource(unsigned int shader, int count, const char** string,
                  const int* length) -> Expected<void> {
  glShaderSource(shader, count, string, length);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glShaderSource failed with error code " << error << '\n';
    switch (error) {
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
    }
  }

  return {};
}

auto UseProgram(unsigned int program) -> Expected<void> {
  glUseProgram(program);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glUseProgram failed with error code " << error << '\n';
    switch (error) {
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
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
  Expected<GLenum> gl_type = ConvertGLDataType(type);
  if (!gl_type) {
    cerr << "ConvertGLDataType failed with error code " << gl_type.error()
         << ": " << gl_type.error().message() << '\n';
    return unexpected(gl_type.error());
  }

  glVertexAttribPointer(index, size, *gl_type, normalized, stride, pointer);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glVertexAttribPointer failed with error code " << error << '\n';
    switch (error) {
        // LCOV_EXCL_START
      default:
        assert(false);  // If we get here, add a new case to the switch.
        return unexpected(MakeErrorCode(kGLError));
        // LCOV_EXCL_STOP
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
