// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <GLFW/glfw3.h>
#include <graphics-engine/engine.h>
#include <graphics-engine/i-shader.h>

#include "gtest/gtest.h"

using enum graphics_engine::gl_types::GLShaderType;

using graphics_engine::engine::InitializeEngine;
using graphics_engine::shader::CreateIShader;
using graphics_engine::types::ShaderSourceMap;

using std::string;

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

TEST_F(ShaderTestFixture, ShaderWorksWithGoodSourceCode) {
  ShaderSourceMap sources = {{kVertex, basic_vs_src},
                             {kFragment, basic_fs_src}};
  auto shader = CreateIShader(sources);
  ASSERT_NE(shader->GetProgramId(), 0);
}

}  // namespace graphics_engine_tests::shader_tests
