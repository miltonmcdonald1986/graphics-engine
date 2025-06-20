// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/shader.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <utility>

#include "error.h"
#include "glad/glad.h"

using enum graphics_engine::gl_types::GLShaderType;
using enum graphics_engine::types::ErrorCode;

using graphics_engine::error::CheckGLError;
using graphics_engine::error::MakeErrorCode;
using graphics_engine::gl_types::GLShaderType;
using graphics_engine::gl_wrappers::AttachShader;
using graphics_engine::gl_wrappers::CreateShader;
using graphics_engine::types::Expected;

using std::cerr;
using std::exception;
using std::format;
using std::is_same_v;
using std::runtime_error;
using std::string;
using std::to_underlying;
using std::unexpected;
using std::unordered_map;
using std::vector;
using std::ranges::contains;
using std::ranges::for_each;
using std::views::keys;

static_assert(is_same_v<GLchar, char>,
              "GLchar and char are not the same type!");
static_assert(is_same_v<GLint, int>, "GLint and int are not the same type!");
static_assert(is_same_v<GLsizei, int>,
              "GLsizei and int are not the same type!");
static_assert(is_same_v<GLuint, unsigned int>,
              "GLuint and unsigned int are not the same type!");

namespace graphics_engine::shader {

auto CompileShader(unsigned int shader_id, const string& source_code)
    -> Expected<void> {
  const GLchar* source_code_cstr = source_code.c_str();
  Expected<void> result =
      ShaderSource(shader_id, 1, &source_code_cstr, nullptr);
  if (!result) {
    cerr << "ShaderSource failed with error code " << result.error().value()
         << ": " << result.error().message() << '\n';
    return unexpected(MakeErrorCode(kShaderError));
  }

  glCompileShader(shader_id);
  CheckGLError();

  GLint compile_status{};
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
  CheckGLError();

  if (compile_status == GL_FALSE) {
    GLint info_log_length{};
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    CheckGLError();

    string info_log(info_log_length, '\0');
    glGetShaderInfoLog(shader_id, info_log_length, nullptr, info_log.data());
    CheckGLError();

    cerr << "Shader compilation failed: " << info_log << '\n';
    return unexpected(MakeErrorCode(kShaderError));
  }

  assert(compile_status == GL_TRUE);

  return {};
}

auto CreateAndCompileShader(gl_types::GLShaderType type,
                            const string& source_code)
    -> Expected<unsigned int> {
  Expected<GLuint> expected_id = CreateShader(type);
  if (!expected_id.has_value()) {
    return unexpected(MakeErrorCode(kShaderError));
  }

  GLuint shader_id = expected_id.value();
  Expected<void> compile_result = CompileShader(shader_id, source_code);
  if (!compile_result.has_value()) {
    return unexpected(MakeErrorCode(kShaderError));
  }

  return shader_id;
}

auto CreateAndLinkShaderProgram(const vector<unsigned int>& shader_ids)
    -> Expected<unsigned int> {
  Expected<unsigned int> program_id = CreateProgram();
  CheckGLError();

  try {
    for_each(shader_ids, [&program_id](GLuint shader_id) {
      if (Expected<void> result{AttachShader(*program_id, shader_id)};
          !result.has_value()) {
        int val = result.error().value();
        constexpr const char* fmt =
            "AttachShader failed with error code {}: {}";
        const string msg = result.error().message();
        throw runtime_error(format(fmt, val, msg));
      }
    });
  } catch (const exception& e) {
    cerr << "Caught std::exception: " << e.what() << '\n';
    return unexpected(MakeErrorCode(kShaderError));
  }

  glLinkProgram(*program_id);
  CheckGLError();

  return program_id;
}

auto CreateProgram() -> Expected<unsigned int> {
  GLuint program_id = glCreateProgram();
  CheckGLError();

  assert(program_id > 0U);

  return program_id;
}

DLLEXPORT [[nodiscard]] auto DeleteShader(unsigned int shader_id)
    -> ::graphics_engine::types::Expected<void> {
  glDeleteShader(shader_id);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glDeleteShader failed with error code " << error << '\n';
    switch (error) {
      default:
        assert(false);  // If we get here, add a new case to the switch
        [[fallthrough]];
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
    }
  }

  return {};
}

auto ShaderSource(unsigned int shader, int count, const char* const* string,
                  const int* length) -> Expected<void> {
  glShaderSource(shader, count, string, length);
  if (GLenum error = glGetError(); error != GL_NO_ERROR) {
    cerr << "glShaderSource failed with error code " << error << '\n';
    switch (error) {
      default:
        assert(false);  // If we get here, add a new case to the switch
        [[fallthrough]];
      case GL_INVALID_VALUE:
        return unexpected(MakeErrorCode(kGLErrorInvalidValue));
      case GL_INVALID_OPERATION:
        return unexpected(MakeErrorCode(kGLErrorInvalidOperation));
    }
  }

  return {};
}

}  // namespace graphics_engine::shader
