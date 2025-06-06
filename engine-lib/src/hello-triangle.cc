// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/hello-triangle.h"

#include <cassert>
#include <iostream>
#include <ranges>
#include <unordered_map>

#include "glad/glad.h"

#include "error.h"
#include "graphics-engine/shader.h"

using enum ::graphics_engine::types::ErrorCode;
using ::graphics_engine::error::MakeErrorCode;
using ::graphics_engine::shader::CompileShader;
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
  Expected<GLuint> expected_vertex_shader_id = CreateShader(kVertex);
  if (!expected_vertex_shader_id.has_value()) {
    cerr << "Failed to create vertex shader: "
         << expected_vertex_shader_id.error().message() << '\n';
    return unexpected(MakeErrorCode(kSceneInitFailure));
  }

  GLuint vertex_shader_id = expected_vertex_shader_id.value();
  const string vertex_shader_source = R"(#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";

  Expected<void> expected_compilation_resut =
      CompileShader(vertex_shader_id, vertex_shader_source);
  if (!expected_compilation_resut.has_value()) {
    cerr << "Vertex shader compilation failed: "
         << expected_compilation_resut.error().message() << '\n';
    return unexpected(MakeErrorCode(kSceneInitFailure));
  }

  Expected<GLuint> expected_fragment_shader_id = CreateShader(kFragment);
  if (!expected_fragment_shader_id.has_value()) {
    cerr << "Failed to create fragment shader: "
         << expected_fragment_shader_id.error().message() << '\n';
    return unexpected(MakeErrorCode(kSceneInitFailure));
  }

  GLuint fragment_shader_id = expected_fragment_shader_id.value();
  const string fragment_shader_source = R"(#version 330 core
out vec4 FragColor;
void main()
{
  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";

  expected_compilation_resut =
      CompileShader(fragment_shader_id, fragment_shader_source);
  if (!expected_compilation_resut.has_value()) {
    cerr << "Fragment shader compilation failed: "
         << expected_compilation_resut.error().message() << '\n';
    return unexpected(MakeErrorCode(kSceneInitFailure));
  }

  shader_program_ = glCreateProgram();
  assert(glGetError() == GL_NO_ERROR);

  glAttachShader(shader_program_, vertex_shader_id);
  assert(glGetError() == GL_NO_ERROR);

  glAttachShader(shader_program_, fragment_shader_id);
  assert(glGetError() == GL_NO_ERROR);

  glLinkProgram(shader_program_);
  assert(glGetError() == GL_NO_ERROR);

  GLint link_status{};
  glGetProgramiv(shader_program_, GL_LINK_STATUS, &link_status);
  assert(glGetError() == GL_NO_ERROR);
  assert(link_status == GL_TRUE);

  glDeleteShader(vertex_shader_id);
  assert(glGetError() == GL_NO_ERROR);

  glDeleteShader(fragment_shader_id);
  assert(glGetError() == GL_NO_ERROR);

  float vertices[] = {
      -0.5f, -0.5f, 0.0f,  // left
      0.5f,  -0.5f, 0.0f,  // right
      0.0f,  0.5f,  0.0f   // top
  };

  GLuint vbo{};
  glGenVertexArrays(1, &vbo);
  glGenBuffers(1, &vao_);
  
  glBindVertexArray(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  return {};
}

auto HelloTriangle::Render() -> Expected<void> {
  glUseProgram(shader_program_);
  glBindVertexArray(
      vao_);  // seeing as we only have a single VAO there's no need to bind it
              // every time, but we'll do so to keep things a bit more organized
  glDrawArrays(GL_TRIANGLES, 0, 3);

  return {};
}

}  // namespace graphics_engine::hello_triangle
