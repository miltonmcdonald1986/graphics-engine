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

using ::std::array;
using ::std::byte;
using ::std::equal;
using ::std::is_same_v;
using ::std::optional;
using ::std::span;
using ::std::unexpected;
using ::std::vector;
using ::std::filesystem::path;
using ::std::filesystem::temp_directory_path;

namespace graphics_engine::engine {

auto AreIdentical(const path& png0, const path& png1) -> Expected<bool> {
  int width1{};
  int height1{};
  int channels1{};
  int width2{};
  int height2{};
  int channels2{};

  const std::string str_filename0 = png0.string();
  const char* filename0 = str_filename0.c_str();
  
  const std::string str_filename1 = png1.string();
  const char* filename1 = str_filename1.c_str();

  stbi_uc* img1 = stbi_load(filename0, &width1, &height1, &channels1, 0);
  stbi_uc* img2 = stbi_load(filename1, &width2, &height2, &channels2, 0);

  if ((img1 == nullptr) || (img2 == nullptr)) {
    return unexpected(MakeErrorCode(kStbErrorLoad));
  }

  if (width1 != width2 || height1 != height2 || channels1 != channels2) {
    return false;
  }

  auto sz_width1 = static_cast<size_t>(width1);
  auto sz_height1 = static_cast<size_t>(height1);
  auto sz_channels1 = static_cast<size_t>(channels1);
  size_t size = sz_width1 * sz_height1 * sz_channels1;
  span<const stbi_uc> span1(img1, size);
  span<const stbi_uc> span2(img2, size);
  bool identical = equal(span1.begin(), span1.end(), span2.begin());

  stbi_image_free(img1);
  stbi_image_free(img2);

  return identical;
}

auto CaptureScreenshot(const optional<path>& dest) -> Expected<void> {
  const path png_path = dest.value_or(temp_directory_path() / "screenshot.png");

  array<GLint, 4> viewport{};
  glGetIntegerv(GL_VIEWPORT, viewport.data());
  assert(glGetError() == GL_NO_ERROR);

  static_assert(is_same_v<GLsizei, GLint>,
                "GLsizei and GLint are not the same type!");
  const GLsizei width = viewport[2];
  const GLsizei height = viewport[3];

  auto sz_width = static_cast<size_t>(width);
  auto sz_height = static_cast<size_t>(height);
  size_t num_channels{4};
  size_t size = sz_width * sz_height * num_channels;
  vector<byte> pixels(size);
  glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
  assert(glGetError() == GL_NO_ERROR);

  const std::string str_filename = png_path.string();
  const char* filename = str_filename.c_str();
  void* data = pixels.data();
  if (stbi_write_png(filename, width, height, 4, data, width * 4) == 0) {
    return unexpected(MakeErrorCode(kStbErrorWritePng));
  }

  return {};
}

auto InitializeEngine() -> Expected<void> {
  if (gladLoadGL() == 0) {
    return unexpected(MakeErrorCode(kEngineInitializationFailed));
  }

  return {};
}

auto Render() -> Expected<void> {
  glClear(GL_COLOR_BUFFER_BIT);
  assert(glGetError() == GL_NO_ERROR);
  return {};
}

auto SetBackgroundColor(const vec4& color) -> void {
  glClearColor(color[0], color[1], color[2], color[3]);
}

}  // namespace graphics_engine::engine
