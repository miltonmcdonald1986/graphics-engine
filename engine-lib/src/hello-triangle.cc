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

using enum ::graphics_engine::types::ErrorCode;

using ::graphics_engine::error::CheckGLError;
using ::graphics_engine::error::MakeErrorCode;
using ::graphics_engine::gl_wrappers::GenVertexArrays;
using ::graphics_engine::shader::CompileShader;
using ::graphics_engine::shader::DeleteShader;
using ::graphics_engine::shader::CreateAndLinkShaderProgram;
using ::graphics_engine::shader::CreateShader;
using enum ::graphics_engine::shader::ShaderType;
using ::graphics_engine::types::Expected;

using ::std::cerr;
using ::std::is_same_v;
using ::std::string;
using ::std::unexpected;

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

  glBindVertexArray(vao_);

  GLuint vbo{};
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  return {};
}

auto HelloTriangle::Render() const -> Expected<void> {
  glUseProgram(this->shader_program_);
  glBindVertexArray(
      vao_);  // seeing as we only have a single VAO there's no need to bind it
              // every time, but we'll do so to keep things a bit more organized
  glDrawArrays(GL_TRIANGLES, 0, 3);

  return {};
}

}  // namespace graphics_engine::hello_triangle
