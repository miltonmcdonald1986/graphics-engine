// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <fstream>
#include <iostream>
#include <vector>

#include "GLFW/glfw3.h"
#include "graphics-engine/engine.h"
#include "graphics-engine/hello-triangle.h"
#include "graphics-engine/image.h"
#include "gtest/gtest.h"

using ::glm::vec4;

using ::graphics_engine::engine::InitializeEngine;
using ::graphics_engine::engine::Render;
using ::graphics_engine::engine::SetBackgroundColor;
using ::graphics_engine::hello_triangle::HelloTriangle;
using ::graphics_engine::image::AreIdentical;
using ::graphics_engine::image::CaptureScreenshot;
using ::graphics_engine::types::Expected;

using ::std::ifstream;
using ::std::ofstream;
using ::std::filesystem::exists;
using ::std::filesystem::path;
using ::std::filesystem::permissions;
using enum ::std::filesystem::perms;
using ::std::filesystem::remove;
using ::std::filesystem::temp_directory_path;

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestInfo;
using ::testing::UnitTest;

namespace graphics_engine_tests::engine_tests {

class EngineTestFixture : public Test {
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

    auto result = InitializeEngine();
    ASSERT_TRUE(result.has_value());
  }

  static void TearDownTestSuite() {
    glfwTerminate();
    int error = glfwGetError(nullptr);
    ASSERT_EQ(error, GLFW_NO_ERROR);
  }

 private:
  static GLFWwindow* window_;
};

GLFWwindow* EngineTestFixture::window_ = nullptr;

TEST(EngineTests, AreIdenticalWorksWithIdenticalFiles) {
  Expected<bool> expected_comparison =
      AreIdentical(path("screenshots/hello-window.png"),
                   path("screenshots/hello-window-copy.png"));
  ASSERT_TRUE(expected_comparison.has_value());
  ASSERT_TRUE(expected_comparison.value());
}

TEST(EngineTests, AreIdenticalWorksWithDifferentFiles) {
  Expected<bool> expected_comparison =
      AreIdentical(path("screenshots/hello-window.png"),
                   path("screenshots/hello-window-modified.png"));
  ASSERT_TRUE(expected_comparison.has_value());
  ASSERT_FALSE(expected_comparison.value());
}

TEST(EngineTests, AreIdenticalFailsWithNonexistentFile) {
  Expected<bool> expected_comparison =
      AreIdentical(path("screenshots/hello-window.png"),
                   path("screenshots/file-that-does-not-exist.png"));
  ASSERT_FALSE(expected_comparison.has_value());
  ASSERT_EQ(expected_comparison.error().message(),
            "Stb Error: Failed to load file.");
}

TEST(EngineTests, AreIdenticalWorksWithDifferentSizedFiles) {
  Expected<bool> expected_comparison =
      AreIdentical(path("screenshots/hello-window.png"),
                   path("screenshots/hello-window-resized.png"));
  ASSERT_TRUE(expected_comparison.has_value());
  ASSERT_FALSE(expected_comparison.value());
}

TEST(EngineTests, InitializeEngineNoContext) {
  auto result = InitializeEngine();
  ASSERT_FALSE(result.has_value());
  ASSERT_STREQ(result.error().category().name(), "graphics_engine::error");
  ASSERT_EQ(result.error().value(), 1);
  ASSERT_EQ(result.error().message(), "glad failed to load OpenGL.");
}

TEST_F(EngineTestFixture, CaptureScreenshotFailsIfItCantWriteTheFile) {
  // Create a listener to clean up the file after the test.
  class CleanUp : public EmptyTestEventListener {
   public:
    CleanUp(const path& path) : path_(path) {}
    void OnTestEnd(const TestInfo&) override { remove(path_); }

   private:
    path path_{};
  };

  const path file{temp_directory_path() / "screenshot.png"};
  UnitTest::GetInstance()->listeners().Append(new CleanUp(file));

  // Create a file.
  ofstream f_out(file);
  f_out << "Hello world";
  f_out.close();
  permissions(file, owner_read | group_read | others_read);

  Expected<void> expected_path{CaptureScreenshot()};
  ASSERT_FALSE(expected_path.has_value());
  ASSERT_STREQ(expected_path.error().category().name(),
               "graphics_engine::error");
  ASSERT_EQ(expected_path.error().message(),
            "Stb Error: Failed to write png file.");
}

TEST_F(EngineTestFixture, SetBackgroundColor) {
  SetBackgroundColor(vec4{0.2F, 0.3F, 0.3F, 1.0F});
  Expected<void> expected_result =
      Render().and_then([]() { return CaptureScreenshot(); });
  ASSERT_TRUE(expected_result.has_value());

  const path& pngPath = temp_directory_path() / "screenshot.png";
  ASSERT_TRUE(exists(pngPath));

  path test_screenshot{"screenshots/hello-window.png"};
  Expected<bool> expected_comparison = AreIdentical(test_screenshot, pngPath);
  ASSERT_TRUE(expected_comparison.has_value());
  ASSERT_TRUE(expected_comparison.value());
}

//TEST_F(EngineTestFixture, Sandbox) {
//  SetBackgroundColor(vec4{0.2F, 0.3F, 0.3F, 1.0F});
//
//  HelloTriangle scene;
//  Expected<void> draw_result =
//      scene.Initialize()
//          .and_then([&scene]() { return scene.Render(); })
//          .and_then([]() { return CaptureScreenshot(); });
//  ASSERT_TRUE(draw_result.has_value());
//}

}  // namespace graphics_engine_tests::engine_tests

int main(int argc, char** argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
