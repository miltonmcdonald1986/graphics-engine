// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "GLFW/glfw3.h"
#include "graphics-engine/engine.h"
#include "graphics-engine/hello-triangle.h"
#include "graphics-engine/shader.h"
#include "gtest/gtest.h"

using ::graphics_engine::engine::InitializeEngine;
using ::graphics_engine::hello_triangle::HelloTriangle;
using ::graphics_engine::shader::CompileShader;
using ::graphics_engine::shader::CreateShader;
using enum ::graphics_engine::shader::ShaderType;
using enum ::graphics_engine::types::ErrorCode;
using ::graphics_engine::types::Expected;

using ::std::string;

using ::testing::Test;

namespace graphics_engine_tests::shader_tests {

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

  /// @brief num_shaders_created_ var tracks the number of shaders that have
  /// been created because the order in which the tests are run affects the
  /// identifier of the created shaders.
  static unsigned int num_shaders_created_;
};

unsigned int ShaderTestFixture::num_shaders_created_ = 0;

TEST_F(ShaderTestFixture, CompileShaderWorksWithGoodVertexShaderSourceCode) {
  Expected<unsigned int> expeted_vertex_shader_id = CreateShader(kVertex);
  num_shaders_created_++;
  ASSERT_TRUE(expeted_vertex_shader_id.has_value());
  ASSERT_EQ(expeted_vertex_shader_id.value(), num_shaders_created_);

  unsigned int vertex_shader_id = expeted_vertex_shader_id.value();
  const string vertex_shader_source = R"(#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";
  auto compile_result = CompileShader(vertex_shader_id, vertex_shader_source);
  ASSERT_TRUE(compile_result.has_value());
}

TEST_F(ShaderTestFixture, CompileShaderFailsWithBadVertexShaderSourceCode) {
  Expected<unsigned int> expeted_vertex_shader_id = CreateShader(kVertex);
  num_shaders_created_++;
  ASSERT_TRUE(expeted_vertex_shader_id.has_value());
  ASSERT_EQ(expeted_vertex_shader_id.value(), num_shaders_created_);

  unsigned int vertex_shader_id = expeted_vertex_shader_id.value();
  const std::string vertex_shader_source = R"(#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
  gl_Poosition = vec4(aPos.x, aPos.y, aPos.z, 1.0); // spelled gl_Position wrong
})";
  auto compile_result = CompileShader(vertex_shader_id, vertex_shader_source);
  ASSERT_FALSE(compile_result.has_value());
}

TEST_F(ShaderTestFixture, CompileShaderFailsWithInvalidOpenGLId) {
  const std::string vertex_shader_source = R"(#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
  gl_Poosition = vec4(aPos.x, aPos.y, aPos.z, 1.0); // spelled gl_Position wrong
})";
  auto compile_result = CompileShader(0, vertex_shader_source);
  ASSERT_FALSE(compile_result.has_value());
}

TEST_F(ShaderTestFixture, CreateShaderWorksWithFragmentShaderType) {
  Expected<unsigned int> create_shader_result = CreateShader(kFragment);
  num_shaders_created_++;
  ASSERT_TRUE(create_shader_result.has_value());
  ASSERT_EQ(create_shader_result.value(), num_shaders_created_);
}

TEST_F(ShaderTestFixture, CreateShaderWorksWithGeometryShaderType) {
  Expected<unsigned int> create_shader_result = CreateShader(kGeometry);
  num_shaders_created_++;
  ASSERT_TRUE(create_shader_result.has_value());
  ASSERT_EQ(create_shader_result.value(), num_shaders_created_);
}

TEST_F(ShaderTestFixture, CreateShaderWorksWithVertexShaderType) {
  Expected<unsigned int> create_shader_result = CreateShader(kVertex);
  num_shaders_created_++;
  ASSERT_TRUE(create_shader_result.has_value());
  ASSERT_EQ(create_shader_result.value(), num_shaders_created_);
}

TEST_F(ShaderTestFixture, CreateShaderFailsWithCounter) {
  Expected<unsigned int> create_shader_result = CreateShader(kNumShaderTypes);
  ASSERT_FALSE(create_shader_result.has_value());
  ASSERT_STREQ(create_shader_result.error().message().c_str(),
               "An invalid shader type was provided.");
  ASSERT_EQ(create_shader_result.error().value(),
            static_cast<int>(kInvalidShaderType));
}

}  // namespace graphics_engine_tests::shader_tests
