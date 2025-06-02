// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "GLFW/glfw3.h"
#include "graphics-engine/engine.h"
#include "gtest/gtest.h"

using ::graphics_engine::engine::InitializeEngine;

class GLFWTestFixture : public ::testing::Test {
 public:
  static void SetUpTestSuite() {
    ASSERT_EQ(glfwInit(), GLFW_TRUE);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    window_ = glfwCreateWindow(640, 480, "", nullptr, nullptr);
    ASSERT_NE(window_, nullptr);

    glfwMakeContextCurrent(window_);
  }

  static void TearDownTestSuite() { glfwTerminate(); }

 private:
  static GLFWwindow* window_;
};

GLFWwindow* GLFWTestFixture::window_ = nullptr;

TEST(EngineTests, InitializeEngineNoContext) {
  auto result = InitializeEngine();
  ASSERT_FALSE(result.has_value());
  ASSERT_STREQ(result.error().category().name(), "graphics_engine::error");
  ASSERT_EQ(result.error().value(), 1);
  ASSERT_EQ(result.error().message(), "Engine Initialization failed.");
}

TEST_F(GLFWTestFixture, InitializeEngineWithContext) {
  auto result = InitializeEngine();
  ASSERT_TRUE(result.has_value());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
