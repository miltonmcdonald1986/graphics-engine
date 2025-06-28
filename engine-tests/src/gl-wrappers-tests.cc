// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <GLFW/glfw3.h>
#include <graphics-engine/engine.h>
#include <graphics-engine/gl-wrappers.h>
#include <gtest/gtest.h>

using graphics_engine::engine::InitializeEngine;
using graphics_engine::gl_types::GLBufferTarget;
using graphics_engine::gl_wrappers::AttachShader;
using graphics_engine::gl_wrappers::BindBuffer;
using graphics_engine::gl_wrappers::CreateProgram;
using graphics_engine::gl_wrappers::CreateShader;
using graphics_engine::gl_wrappers::GenBuffers;
using graphics_engine::types::Expected;
using std::string;
using testing::Test;

using enum graphics_engine::gl_types::GLBufferTarget;
using enum graphics_engine::gl_types::GLShaderType;
using enum graphics_engine::types::ErrorCode;

namespace graphics_engine_tests::gl_wrappers_tests {

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

struct GLWrappersTestFixture : public Test {
  static void SetUpTestSuite() {
    ASSERT_EQ(glfwInit(), GLFW_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

TEST_F(GLWrappersTestFixture, AttachShaderWorks) {
  // Trash is handled.
  Expected<void> result = AttachShader(0, 0);
  ASSERT_FALSE(result);
  ASSERT_EQ(result.error().value(), 5);
  ASSERT_EQ(result.error().message(), "OpenGL Error: Invalid Value.");

  // Make real shader/program for testing.
  Expected<unsigned> shader_id{CreateShader(kVertex)};
  ASSERT_TRUE(shader_id);

  Expected<unsigned> program_id{CreateProgram()};
  ASSERT_TRUE(program_id);

  // Call the function with the inputs in the wrong order.
  result = AttachShader(*shader_id, *program_id);
  ASSERT_FALSE(result);
  ASSERT_EQ(result.error().value(), 4);
  ASSERT_EQ(result.error().message(), "OpenGL Error: Invalid Operation.");

  // Call the function with the inputs in the correct order.
  result = AttachShader(*program_id, *shader_id);
  ASSERT_TRUE(result);
}

TEST_F(GLWrappersTestFixture, BindBufferWorks) {
  EXPECT_DEBUG_DEATH((void)BindBuffer(static_cast<GLBufferTarget>(42), 0), "");

  // zero here is actually legal, as buffer set to zero effectively unbinds any
  // buffer object previously bound, and restores client memory usage for that
  // buffer object target
  ASSERT_TRUE(BindBuffer(kArray, 0));

  Expected<void> result{BindBuffer(kArray, 2025)};
  ASSERT_FALSE(result);
  ASSERT_EQ(result.error().value(), 4);
  ASSERT_EQ(result.error().message(), "OpenGL Error: Invalid Operation.");

  // Let's actually gen a real buffer and bind it.
  unsigned buffer;
  result = GenBuffers(1, &buffer);
  ASSERT_TRUE(result);
}

}  // namespace graphics_engine_tests::gl_wrappers_tests
