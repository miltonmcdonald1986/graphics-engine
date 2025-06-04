// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <fstream>
#include <iostream>
#include <vector>

#include "GLFW/glfw3.h"
#include "graphics-engine/engine.h"
#include "gtest/gtest.h"

using ::glm::vec4;

using ::graphics_engine::engine::AreIdentical;
using ::graphics_engine::engine::CaptureScreenshot;
using ::graphics_engine::engine::InitializeEngine;
using ::graphics_engine::engine::Render;
using ::graphics_engine::engine::SetBackgroundColor;
using ::graphics_engine::types::Expected;

using ::std::ifstream;
using ::std::filesystem::path;
using ::std::filesystem::temp_directory_path;

using ::testing::InitGoogleTest;
using ::testing::Test;

class GLFWTestFixture : public Test {
 public:
  static void SetUpTestSuite() {
    ASSERT_EQ(glfwInit(), GLFW_TRUE);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    int error = glfwGetError(nullptr);
    ASSERT_EQ(error, GLFW_NO_ERROR);

    window_ = glfwCreateWindow(640, 480, "", nullptr, nullptr);
    ASSERT_NE(window_, nullptr);

    glfwMakeContextCurrent(window_);
    error = glfwGetError(nullptr);
    ASSERT_EQ(error, GLFW_NO_ERROR);
  }

  static void TearDownTestSuite() {
    glfwTerminate();
    int error = glfwGetError(nullptr);
    ASSERT_EQ(error, GLFW_NO_ERROR);
  }

 private:
  static GLFWwindow* window_;
};

GLFWwindow* GLFWTestFixture::window_ = nullptr;

TEST(EngineTests, AreIdenticalWorksWithIdenticalFiles) {
  Expected<bool> expectedComparison =
      AreIdentical(path("screenshots/hello-window.png"),
                   path("screenshots/hello-window-copy.png"));
  ASSERT_TRUE(expectedComparison.has_value());
  ASSERT_TRUE(expectedComparison.value());
}

TEST(EngineTests, AreIdenticalWorksWithDifferentFiles) {
  Expected<bool> expectedComparison =
      AreIdentical(path("screenshots/hello-window.png"),
                   path("screenshots/hello-window-modified.png"));
  ASSERT_TRUE(expectedComparison.has_value());
  ASSERT_FALSE(expectedComparison.value());
}

TEST(EngineTests, AreIdenticalFailsWithNonexistentFile) {
  Expected<bool> expectedComparison =
      AreIdentical(path("screenshots/hello-window.png"),
                   path("screenshots/file-that-does-not-exist.png"));
  ASSERT_FALSE(expectedComparison.has_value());
  ASSERT_EQ(expectedComparison.error().message(),
            "Stb Error: Failed to load file.");
}

TEST(EngineTests, AreIdenticalWorksWithDifferentSizedFiles) {
  Expected<bool> expectedComparison =
      AreIdentical(path("screenshots/hello-window.png"),
                   path("screenshots/hello-window-resized.png"));
  ASSERT_TRUE(expectedComparison.has_value());
  ASSERT_FALSE(expectedComparison.value());
}

TEST(EngineTests, InitializeEngineNoContext) {
  auto result = InitializeEngine();
  ASSERT_FALSE(result.has_value());
  ASSERT_STREQ(result.error().category().name(), "graphics_engine::error");
  ASSERT_EQ(result.error().value(), 1);
  ASSERT_EQ(result.error().message(), "Engine Initialization failed.");
}

TEST_F(GLFWTestFixture, CaptureScreenshotFailsIfItCantWriteTheFile) {
  auto result = InitializeEngine();
  ASSERT_TRUE(result.has_value());

  // Open a file in read only mode so Capturing the screenshot fails
  ifstream file(temp_directory_path() /
                "screenshot.png");  // Open in read-only mode
  ASSERT_TRUE(file);

  Expected<path> expectedPath{CaptureScreenshot()};
  ASSERT_FALSE(expectedPath.has_value());
  ASSERT_STREQ(expectedPath.error().category().name(),
               "graphics_engine::error");
  ASSERT_EQ(expectedPath.error().message(),
            "Stb Error: Failed to write png file.");
}

TEST_F(GLFWTestFixture, InitializeEngineWithContext) {
  auto result = InitializeEngine();
  ASSERT_TRUE(result.has_value());
}

TEST_F(GLFWTestFixture, SetBackgroundColor) {
  auto result = InitializeEngine();
  ASSERT_TRUE(result.has_value());

  SetBackgroundColor(vec4{0.2F, 0.3F, 0.3F, 1.0F});
  Expected<path> expectedPath =
      Render().and_then([]() { return CaptureScreenshot(); });
  ASSERT_TRUE(expectedPath.has_value());

  const path& pngPath = expectedPath.value();
  ASSERT_EQ(pngPath.string(),
            (temp_directory_path() / "screenshot.png").string());

  Expected<bool> expectedComparison =
      AreIdentical(path("screenshots/hello-window.png"), pngPath);
  ASSERT_TRUE(expectedComparison.has_value());
  ASSERT_TRUE(expectedComparison.value());
}

// TEST_F(GLFWTestFixture, Sandbox) {
//  Uncomment and start coding in here to just play around!
// }

int main(int argc, char** argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
