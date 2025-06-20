// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "scene-hello-triangle.h"

#include <array>
#include <cassert>

#include "graphics-engine/gl-types.h"
#include "graphics-engine/shader.h"

using enum graphics_engine::gl_types::GLBufferTarget;
using enum graphics_engine::gl_types::GLDataType;
using enum graphics_engine::gl_types::GLDataUsagePattern;
using enum graphics_engine::gl_types::GLDrawMode;
using enum graphics_engine::gl_types::GLShaderType;

using graphics_engine::gl_wrappers::BindBuffer;
using graphics_engine::gl_wrappers::BindVertexArray;
using graphics_engine::gl_wrappers::BufferData;
using graphics_engine::gl_wrappers::DrawArrays;
using graphics_engine::gl_wrappers::EnableVertexAttribArray;
using graphics_engine::gl_wrappers::GenBuffers;
using graphics_engine::gl_wrappers::GenVertexArrays;
using graphics_engine::gl_wrappers::UseProgram;
using graphics_engine::gl_wrappers::VertexAttribPointer;
using graphics_engine::shader::CreateAndCompileShader;
using graphics_engine::shader::CreateAndLinkShaderProgram;
using graphics_engine::shader::DeleteShader;
using graphics_engine::types::Expected;

using std::string;

namespace demo_app::scene_hello_triangle {

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

  Expected<unsigned int> vs_id = CreateAndCompileShader(kVertex, vs_src);
  assert(vs_id.has_value());

  Expected<unsigned int> fs_id = CreateAndCompileShader(kFragment, fs_src);
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

  unsigned int vbo{};
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
  HelloTrianglePtr hello_triangle(new HelloTriangle());
  Expected<void> result = hello_triangle->Initialize();
  if (!result.has_value()) {
    return nullptr;
  }

  return hello_triangle;
}

}  // namespace graphics_engine::hello_triangle
