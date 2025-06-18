// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/hello-triangle.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <unordered_map>

#include "error.h"
#include "glad/glad.h"
#include "graphics-engine/gl-wrappers.h"
#include "graphics-engine/shader.h"

using enum graphics_engine::types::ErrorCode;
using enum graphics_engine::gl_wrappers::GLBufferTarget;
using enum graphics_engine::gl_wrappers::GLDataType;
using enum graphics_engine::gl_wrappers::GLDataUsagePattern;
using enum graphics_engine::gl_wrappers::GLDrawMode;
using enum graphics_engine::gl_wrappers::GLShaderType;

using graphics_engine::error::CheckGLError;
using graphics_engine::error::MakeErrorCode;
using graphics_engine::gl_wrappers::BindBuffer;
using graphics_engine::gl_wrappers::BindVertexArray;
using graphics_engine::gl_wrappers::BufferData;
using graphics_engine::gl_wrappers::CreateShader;
using graphics_engine::gl_wrappers::DrawArrays;
using graphics_engine::gl_wrappers::EnableVertexAttribArray;
using graphics_engine::gl_wrappers::GenBuffers;
using graphics_engine::gl_wrappers::GenVertexArrays;
using graphics_engine::gl_wrappers::UseProgram;
using graphics_engine::gl_wrappers::VertexAttribPointer;
using graphics_engine::shader::CompileShader;
using graphics_engine::shader::CreateAndCompileShader;
using graphics_engine::shader::CreateAndLinkShaderProgram;
using graphics_engine::shader::DeleteShader;
using graphics_engine::types::Expected;

using std::cerr;
using std::is_same_v;
using std::string;
using std::unexpected;

static_assert(is_same_v<GLchar, char>,
              "GLchar and char are not the same type!");
static_assert(is_same_v<GLuint, unsigned int>,
              "GLuint and unsigned int are not the same type!");

namespace graphics_engine::hello_triangle {

auto HelloTriangle::Initialize() -> Expected<void> {
  const string vs_src = R"(#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";

  const string fs_src = R"(#version 330 core
out vec4 FragColor;
void main()
{
  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";

  Expected<GLuint> vs_id = CreateAndCompileShader(kVertex, vs_src);
  assert(vs_id.has_value());

  Expected<GLuint> fs_id = CreateAndCompileShader(kFragment, fs_src);
  assert(fs_id.has_value());

  auto program_id = CreateAndLinkShaderProgram({*vs_id, *fs_id});
  assert(program_id.has_value());

  shader_program_ = *program_id;

  Expected<void> result = DeleteShader(*vs_id);
  assert(result.has_value());

  result = DeleteShader(*fs_id);
  assert(result.has_value());

  const std::array<float, 9> vertices = {
      -0.5F, -0.5F, 0.0F,  // left
      0.5F,  -0.5F, 0.0F,  // right
      0.0F,  0.5F,  0.0F   // top
  };

  result = GenVertexArrays(1, &vao_);
  if (!result.has_value()) {
    assert(false);
    return std::unexpected(result.error());
  }

  result = BindVertexArray(vao_);
  if (!result.has_value()) {
    assert(false);
    return std::unexpected(result.error());
  }

  GLuint vbo{};
  result = GenBuffers(1, &vbo);
  if (!result.has_value()) {
    assert(false);
    return std::unexpected(result.error());
  }

  result = BindBuffer(kArray, vbo);
  if (!result.has_value()) {
    assert(false);
    return std::unexpected(result.error());
  }

  result = BufferData(kArray, sizeof(vertices), vertices.data(), kStaticDraw);
  if (!result.has_value()) {
    assert(false);
    return std::unexpected(result.error());
  }

  result = VertexAttribPointer(0, 3, kFloat, 0, 3 * sizeof(float), nullptr);
  if (!result.has_value()) {
    assert(false);
    return std::unexpected(result.error());
  }

  result = EnableVertexAttribArray(0);
  if (!result.has_value()) {
    assert(false);
    return std::unexpected(result.error());
  }

  return {};
}

auto HelloTriangle::Render() const -> Expected<void> {
  Expected<void> result = UseProgram(this->shader_program_);
  if (!result.has_value()) {
    assert(false);
    return std::unexpected(result.error());
  }

  result = BindVertexArray(vao_);
  if (!result.has_value()) {
    assert(false);
    return std::unexpected(result.error());
  }

  result = DrawArrays(kTriangles, 0, 3);
  if (!result.has_value()) {
    assert(false);
    return std::unexpected(result.error());
  }

  return {};
}

auto CreateHelloTriangleScene() -> HelloTrianglePtr {
  HelloTrianglePtr hello_triangle = std::make_unique<HelloTriangle>();
  if (!hello_triangle) {
    return nullptr;
  }

  Expected<void> result = hello_triangle->Initialize();
  if (!result.has_value()) {
    return nullptr;
  }

  return hello_triangle;
}

}  // namespace graphics_engine::hello_triangle
