// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "GLFW/glfw3.h"
#include "graphics-engine/engine.h"
#include "graphics-engine/gl-wrappers.h"
#include "graphics-engine/hello-triangle.h"
#include "graphics-engine/shader.h"
#include "gtest/gtest.h"

using enum graphics_engine::gl_wrappers::GLShaderType;
using enum graphics_engine::types::ErrorCode;

using graphics_engine::engine::InitializeEngine;
using graphics_engine::gl_wrappers::AttachShader;
using graphics_engine::gl_wrappers::CreateShader;
using graphics_engine::gl_wrappers::GLShaderType;
using graphics_engine::shader::CompileShader;
using graphics_engine::shader::CreateAndCompileShader;
using graphics_engine::shader::CreateAndLinkShaderProgram;
using graphics_engine::shader::CreateProgram;
using graphics_engine::shader::ShaderSource;
using graphics_engine::types::Expected;

using std::string;
using std::to_underlying;

using testing::Test;

namespace graphics_engine_tests::shader_tests {

namespace {

inline const string basic_vs_src = R"(#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";

inline const string basic_vs_src_spelling_err = R"(#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
  gl_Poosition = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";

inline const string basic_fs_src = R"(#version 330 core
out vec4 FragColor;
void main()
{
  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";

}  // namespace

struct ShaderTestFixture : public Test {
  static void SetUpTestSuite() {
    ASSERT_EQ(glfwInit(), GLFW_TRUE);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    int error = glfwGetError(nullptr);
    ASSERT_EQ(error, GLFW_NO_ERROR);

    GLFWwindow* window = glfwCreateWindow(640, 480, "", nullptr, nullptr);
    ASSERT_NE(window, nullptr);

    glfwMakeContextCurrent(window);
    error = glfwGetError(nullptr);
    ASSERT_EQ(error, GLFW_NO_ERROR);

    auto init_engine_result = InitializeEngine();
    ASSERT_TRUE(init_engine_result.has_value());
  }

  static void TearDownTestSuite() {
    glfwTerminate();
    int error = glfwGetError(nullptr);
    ASSERT_EQ(error, GLFW_NO_ERROR);
  }
};

TEST_F(ShaderTestFixture, CompileShaderWorksWithGoodVertexShaderSourceCode) {
  Expected<unsigned int> expeted_vertex_shader_id = CreateShader(kVertex);
  ASSERT_TRUE(expeted_vertex_shader_id.has_value());

  unsigned int vertex_shader_id = expeted_vertex_shader_id.value();
  auto compile_result = CompileShader(vertex_shader_id, basic_vs_src);
  ASSERT_TRUE(compile_result.has_value());
}

TEST_F(ShaderTestFixture, CompileShaderFailsWithBadVertexShaderSourceCode) {
  Expected<unsigned int> expeted_vertex_shader_id = CreateShader(kVertex);
  ASSERT_TRUE(expeted_vertex_shader_id.has_value());

  unsigned int vertex_shader_id = expeted_vertex_shader_id.value();
  auto compile_result =
      CompileShader(vertex_shader_id, basic_vs_src_spelling_err);
  ASSERT_FALSE(compile_result.has_value());
}

TEST_F(ShaderTestFixture, CompileShaderFailsWithInvalidOpenGLId) {
  auto compile_result = CompileShader(0, basic_vs_src);
  ASSERT_FALSE(compile_result.has_value());
}

TEST_F(ShaderTestFixture, CreateShaderWorksWithFragmentShaderType) {
  Expected<unsigned int> create_shader_result = CreateShader(kFragment);
  ASSERT_TRUE(create_shader_result.has_value());
}

TEST_F(ShaderTestFixture, CreateShaderWorksWithGeometryShaderType) {
  Expected<unsigned int> create_shader_result = CreateShader(kGeometry);
  ASSERT_TRUE(create_shader_result.has_value());
}

TEST_F(ShaderTestFixture, CreateShaderWorksWithVertexShaderType) {
  Expected<unsigned int> create_shader_result = CreateShader(kVertex);
  ASSERT_TRUE(create_shader_result.has_value());
}

TEST_F(ShaderTestFixture, CreateShaderFailsWithJunk) {
  ASSERT_DEATH((void)CreateShader(static_cast<GLShaderType>(42)),
               "ConvertGLShaderType failed with underlying value 42");
}

TEST_F(ShaderTestFixture, CreateAndCompileShaderWorksWithGoodData) {
  Expected<unsigned int> shader_id{
      CreateAndCompileShader(kVertex, basic_vs_src)};
  ASSERT_TRUE(shader_id.has_value());
}

TEST_F(ShaderTestFixture, CreateAndCompileShaderFailsWithBadSourceCode) {
  Expected<unsigned int> shader_id{
      CreateAndCompileShader(kVertex, basic_vs_src_spelling_err)};
  ASSERT_FALSE(shader_id.has_value());
  ASSERT_STREQ(shader_id.error().message().c_str(), "Shader Error.");
  ASSERT_EQ(shader_id.error().value(), static_cast<int>(kShaderError));
}

TEST_F(ShaderTestFixture, CreateAndLinkShaderProgramWorksWithGoodData) {
  Expected<unsigned int> vs_id{CreateAndCompileShader(kVertex, basic_vs_src)};
  ASSERT_TRUE(vs_id.has_value());

  Expected<unsigned int> fs_id{CreateAndCompileShader(kFragment, basic_fs_src)};
  ASSERT_TRUE(fs_id.has_value());

  Expected<unsigned int> program_id{
      CreateAndLinkShaderProgram({*vs_id, *fs_id})};
  ASSERT_TRUE(program_id.has_value());
}

TEST_F(ShaderTestFixture, CreateAndLinkShaderProgramFailsWithInvalidData) {
  Expected<unsigned int> program_id{CreateAndLinkShaderProgram({0})};
  ASSERT_FALSE(program_id.has_value());
  ASSERT_STREQ(program_id.error().message().c_str(), "Shader Error.");
  ASSERT_EQ(program_id.error().value(), to_underlying(kShaderError));
}

TEST_F(ShaderTestFixture, AttachShaderWorksWithGoodData) {
  Expected<unsigned int> vs_id{CreateAndCompileShader(kVertex, basic_vs_src)};
  ASSERT_TRUE(vs_id.has_value());

  Expected<unsigned int> fs_id{CreateAndCompileShader(kFragment, basic_fs_src)};
  ASSERT_TRUE(fs_id.has_value());

  Expected<unsigned int> program_id{CreateProgram()};
  ASSERT_TRUE(program_id.has_value());

  Expected<void> result{AttachShader(*program_id, *vs_id)};
  ASSERT_TRUE(result.has_value());

  result = {AttachShader(*program_id, *fs_id)};
  ASSERT_TRUE(result.has_value());
}

TEST_F(ShaderTestFixture, AttachShaderFailsWithInvalidData) {
  Expected<unsigned int> program_id{CreateProgram()};
  ASSERT_TRUE(program_id.has_value());

  Expected<void> result{AttachShader(*program_id, 0)};
  ASSERT_FALSE(result.has_value());
  ASSERT_STREQ(result.error().message().c_str(),
               "OpenGL Error: Invalid Value.");
  ASSERT_EQ(result.error().value(), static_cast<int>(kGLErrorInvalidValue));
}

TEST_F(ShaderTestFixture, AttachShaderFailsToAttachProgram) {
  Expected<unsigned int> program_id{CreateProgram()};
  ASSERT_TRUE(program_id.has_value());

  Expected<void> result{AttachShader(*program_id, *program_id)};
  ASSERT_FALSE(result.has_value());
  ASSERT_STREQ(result.error().message().c_str(),
               "OpenGL Error: Invalid Operation.");
  ASSERT_EQ(result.error().value(), static_cast<int>(kGLErrorInvalidOperation));
}

TEST_F(ShaderTestFixture, ShaderSourceFailsToSourceProgram) {
  Expected<unsigned int> program_id{CreateProgram()};
  ASSERT_TRUE(program_id.has_value());

  const char* cstr = basic_fs_src.c_str();
  Expected<void> result{ShaderSource(*program_id, 1, &cstr, nullptr)};
  ASSERT_FALSE(result.has_value());
  ASSERT_STREQ(result.error().message().c_str(),
               "OpenGL Error: Invalid Operation.");
  ASSERT_EQ(result.error().value(), static_cast<int>(kGLErrorInvalidOperation));
}

}  // namespace graphics_engine_tests::shader_tests
