// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/hello-triangle.h"

#include <cassert>
#include <iostream>

#include "glad/glad.h"

using ::graphics_engine::types::Expected;
using ::std::is_same_v;

static_assert(is_same_v<GLchar, char>,
              "GLchar and char are not the same type!");
static_assert(is_same_v<GLuint, unsigned int>,
              "GLuint and unsigned int are not the same type!");

namespace graphics_engine::hello_triangle {

auto HelloTriangle::Initialize() -> Expected<void> {
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  assert(glGetError() == GL_NO_ERROR);
  assert(vertex_shader > 0);

  const std::string vertex_shader_source =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";

  const GLchar* vertex_shader_source_cstr = vertex_shader_source.c_str();
  glShaderSource(vertex_shader, 1, &vertex_shader_source_cstr, nullptr);
  assert(glGetError() == GL_NO_ERROR);

  glCompileShader(vertex_shader);
  assert(glGetError() == GL_NO_ERROR);

  GLint compile_status{};
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
  assert(glGetError() == GL_NO_ERROR);
  assert(compile_status == GL_TRUE);

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  assert(glGetError() == GL_NO_ERROR);
  assert(fragment_shader > 0);

  const std::string fragment_shader_source =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n\0";

  const GLchar* fragment_shader_source_cstr = fragment_shader_source.c_str();
  glShaderSource(fragment_shader, 1, &fragment_shader_source_cstr, nullptr);
  assert(glGetError() == GL_NO_ERROR);

  glCompileShader(fragment_shader);
  assert(glGetError() == GL_NO_ERROR);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
  assert(glGetError() == GL_NO_ERROR);
  assert(compile_status == GL_TRUE);

  shader_program_ = glCreateProgram();
  assert(glGetError() == GL_NO_ERROR);

  glAttachShader(shader_program_, vertex_shader);
  assert(glGetError() == GL_NO_ERROR);

  glAttachShader(shader_program_, fragment_shader);
  assert(glGetError() == GL_NO_ERROR);

  glLinkProgram(shader_program_);
  assert(glGetError() == GL_NO_ERROR);

  GLint link_status{};
  glGetProgramiv(shader_program_, GL_LINK_STATUS, &link_status);
  assert(glGetError() == GL_NO_ERROR);
  assert(link_status == GL_TRUE);

  glDeleteShader(vertex_shader);
  assert(glGetError() == GL_NO_ERROR);

  glDeleteShader(fragment_shader);
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
