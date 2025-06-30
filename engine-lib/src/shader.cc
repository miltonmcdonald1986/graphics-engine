// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "shader.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <utility>

#include "error.h"
#include "glad/glad.h"
#include "graphics-engine/i-shader.h"

using enum graphics_engine::gl_types::GLShaderObjectParameter;
using enum graphics_engine::gl_types::GLShaderType;
using enum graphics_engine::types::ErrorCode;

using graphics_engine::error::CheckGLError;
using graphics_engine::error::MakeErrorCode;
using graphics_engine::gl_types::GLShaderType;
using graphics_engine::gl_wrappers::AttachShader;
using graphics_engine::gl_wrappers::CompileShader;
using graphics_engine::gl_wrappers::CreateProgram;
using graphics_engine::gl_wrappers::CreateShader;
using graphics_engine::gl_wrappers::DeleteProgram;
using graphics_engine::gl_wrappers::DeleteShader;
using graphics_engine::gl_wrappers::DetachShader;
using graphics_engine::gl_wrappers::GetShaderInfoLog;
using graphics_engine::gl_wrappers::GetShaderiv;
using graphics_engine::gl_wrappers::LinkProgram;
using graphics_engine::gl_wrappers::ShaderSource;
using graphics_engine::shader::IShaderPtr;
using graphics_engine::types::Expected;
using graphics_engine::types::ShaderSourceMap;

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

auto CreateIShader(const ShaderSourceMap& sources) -> IShaderPtr {
  auto shader = std::make_unique<Shader>();
  Expected<void> result = shader->Initialize(sources);
  if (!result.has_value()) {
    cerr << "Shader initialization failed with error code "
         << result.error().value() << ": " << result.error().message() << '\n';
    return nullptr;
  }

  return shader;
}

Shader::~Shader() {
  Expected<void> result = DeleteProgram(program_id_);
  if (!result) {
    cerr << "DeleteProgram failed with error code " << result.error().value()
         << ": " << result.error().message() << '\n';

    // Nothing else to do here; just log the error.
  }
}

auto Shader::GetProgramId() const -> unsigned int { return program_id_; }

auto Shader::Initialize(const types::ShaderSourceMap& sources)
    -> types::Expected<void> {
  std::vector<GLuint> shader_ids;

  // Compile each of the shaders in the shader source map.
  for (const auto& [shader_type, source_code] : sources) {
    Expected<GLuint> shader_id = CreateShader(shader_type);
    if (!shader_id) {
      cerr << "CreateShader failed for shader type "
           << to_underlying(shader_type) << " with error code "
           << shader_id.error().value() << ": " << shader_id.error().message()
           << '\n';
      return unexpected(shader_id.error());
    }

    shader_ids.push_back(*shader_id);

    const GLchar* source_code_cstr = source_code.c_str();
    Expected<void> result =
        ShaderSource(*shader_id, 1, &source_code_cstr, nullptr);
    if (!result.has_value()) {
      cerr << "ShaderSource failed with error code " << result.error().value()
           << ": " << result.error().message() << '\n';
      return unexpected(result.error());
    }

    result = CompileShader(*shader_id);
    if (!result.has_value()) {
      cerr << "CompileShader failed with error code " << result.error().value()
           << ": " << result.error().message() << '\n';
      return unexpected(result.error());
    }

    int params{};
    result = GetShaderiv(*shader_id, kCompileStatus, &params);
    if (!result.has_value()) {
      cerr << "GetShaderiv failed with error code " << result.error().value()
           << ": " << result.error().message() << '\n';
      return unexpected(result.error());
    }

    // If compilation failed...
    if (params == GL_FALSE) {
      result = GetShaderiv(*shader_id, kInfoLogLength, &params);
      if (!result.has_value()) {
        cerr << "GetShaderiv failed with error code " << result.error().value()
             << ": " << result.error().message() << '\n';
        return unexpected(result.error());
      }

      string info_log(params, '\0');
      result = GetShaderInfoLog(*shader_id, params, nullptr, info_log.data());
      if (!result.has_value()) {
        cerr << "GetShaderInfoLog failed with error code "
             << result.error().value() << ": " << result.error().message()
             << '\n';
        return unexpected(result.error());
      }
    }
  }

  // Compilation succeeded. Time to link.
  Expected<GLuint> program_id = CreateProgram();
  if (!program_id) {
    cerr << "CreateProgram failed with error code "
         << program_id.error().value() << ": " << program_id.error().message()
         << '\n';
    return unexpected(program_id.error());
  }

  for (const auto& shader_id : shader_ids) {
    Expected<void> result = AttachShader(*program_id, shader_id);
    if (!result) {
      cerr << "AttachShader failed with error code: " << result.error().value()
           << ": " << result.error().message() << '\n';
      return unexpected(result.error());
    }
  }

  Expected<void> result = LinkProgram(*program_id);
  if (!result) {
    cerr << "LinkProgram failed with error code: " << result.error().value()
         << ": " << result.error().message() << '\n';
    return unexpected(result.error());
  }

  program_id_ = *program_id;

  for (auto shader_id : shader_ids) {
    result = DetachShader(program_id_, shader_id);
    if (!result) {
      cerr << "DetachShader failed with error code: " << result.error().value()
           << ": " << result.error().message() << '\n';
      return unexpected(result.error());
    }

    result = DeleteShader(shader_id);
    if (!result) {
      cerr << "DeleteShader failed with error code: " << result.error().value()
           << ": " << result.error().message() << '\n';
      return unexpected(result.error());
    }
  }

  return {};
}

}  // namespace graphics_engine::shader
