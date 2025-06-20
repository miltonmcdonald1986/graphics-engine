// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_GL_CLEAR_FLAGS_H_
#define ENGINE_LIB_GL_CLEAR_FLAGS_H_

#include <bitset>
#include <utility>

#include "graphics-engine/i-gl-clear-flags.h"

namespace graphics_engine::gl_clear_flags {

class GLClearFlags : public graphics_engine::i_gl_clear_flags::IGLClearFlags {
 public:
  ~GLClearFlags() override = default;
  auto Set(graphics_engine::gl_types::GLClearBit bit) -> GLClearFlags&;
  auto Reset(graphics_engine::gl_types::GLClearBit bit) -> GLClearFlags&;
  auto Test(graphics_engine::gl_types::GLClearBit bit) const -> bool;

 private:
  static constexpr int kExpectedNumClearBits = 3;
  static_assert(
      std::to_underlying(graphics_engine::gl_types::GLClearBit::kNumBits) ==
          kExpectedNumClearBits,
      "Fix value of kExpectedNumClearBits to match GLClearBit definition!");

  std::bitset<kExpectedNumClearBits> flags_;
};

}  // namespace graphics_engine::gl_clear_flags

#endif  // ENGINE_LIB_GL_CLEAR_FLAGS_H_
