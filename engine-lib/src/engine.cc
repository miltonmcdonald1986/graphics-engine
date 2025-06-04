// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/engine.h"

#include <array>
#include <span>

#include "error.h"
#include "glad/glad.h"
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

using ::glm::vec4;

using enum ::graphics_engine::error::ErrorCode;
using ::graphics_engine::types::Expected;

using ::std::filesystem::path;

namespace graphics_engine::engine {

auto AreIdentical(const path& png0, const path& png1) -> Expected<bool> {
  int width1{};
  int height1{};
  int channels1{};
  int width2{};
  int height2{};
  int channels2{};

  stbi_uc* img1 =
      stbi_load(png0.string().c_str(), &width1, &height1, &channels1, 0);
  stbi_uc* img2 =
      stbi_load(png1.string().c_str(), &width2, &height2, &channels2, 0);

  if ((img1 == nullptr) || (img2 == nullptr)) {
    return std::unexpected(MakeErrorCode(kStbErrorLoad));
  }

  if (width1 != width2 || height1 != height2 || channels1 != channels2) {
    return false;
  }

  size_t size = static_cast<size_t>(width1) * static_cast<size_t>(height1) *
                static_cast<size_t>(channels1);
  std::span<const stbi_uc> span1(img1, size);
  std::span<const stbi_uc> span2(img2, size);
  bool identical = std::equal(span1.begin(), span1.end(), span2.begin());

  stbi_image_free(img1);
  stbi_image_free(img2);

  return identical;
}

auto CaptureScreenshot() -> Expected<std::filesystem::path> {
  std::array<GLint, 4> viewport{};  // x, y, width, height
  glGetIntegerv(GL_VIEWPORT, viewport.data());
  GLenum error = glGetError();
  if (error == GL_INVALID_ENUM) {
    return std::unexpected(MakeErrorCode(kGLErrorInvalidEnum));
  }
  if (error == GL_INVALID_VALUE) {
    return std::unexpected(MakeErrorCode(kGLErrorInvalidValue));
  }

  static_assert(std::is_same_v<GLsizei, GLint>,
                "GLsizei and GLint are not the same type!");
  const GLsizei width = viewport[2];
  const GLsizei height = viewport[3];
  size_t size = static_cast<size_t>(width) * static_cast<size_t>(height) *
                static_cast<size_t>(4);
  std::vector<std::byte> pixels(size);
  glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
  error = glGetError();
  if (error == GL_INVALID_ENUM) {
    return std::unexpected(MakeErrorCode(kGLErrorInvalidEnum));
  }
  if (error == GL_INVALID_OPERATION) {
    return std::unexpected(MakeErrorCode(kGLErrorInvalidOperation));
  }
  if (error == GL_INVALID_VALUE) {
    return std::unexpected(MakeErrorCode(kGLErrorInvalidValue));
  }

  const std::filesystem::path tempDir{std::filesystem::temp_directory_path()};
  std::filesystem::path pngPath{tempDir / "screenshot.png"};
  if (stbi_write_png(pngPath.string().c_str(), width, height, 4, pixels.data(),
                     width * 4) == 0) {
    return std::unexpected(MakeErrorCode(kStbErrorWritePng));
  }

  return pngPath;
}

auto InitializeEngine() -> Expected<void> {
  if (gladLoadGL() == 0) {
    return std::unexpected(MakeErrorCode(kEngineInitializationFailed));
  }

  return {};
}

auto Render() -> Expected<void> {
  glClear(GL_COLOR_BUFFER_BIT);
  auto error = glGetError();
  if (error == GL_INVALID_VALUE) {
    return std::unexpected(MakeErrorCode(kGLErrorInvalidValue));
  }

  return {};
}

auto SetBackgroundColor(const vec4& color) -> void {
  glClearColor(color[0], color[1], color[2], color[3]);
}

}  // namespace graphics_engine::engine
