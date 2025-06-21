// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "gl-clear-flags.h"

using graphics_engine::gl_types::GLClearBit;

using std::to_underlying;

namespace graphics_engine::gl_clear_flags {

auto GLClearFlags::Set(GLClearBit bit) -> GLClearFlags& {
  flags_.set(to_underlying(bit));
  return *this;
}

auto GLClearFlags::Reset(GLClearBit bit) -> GLClearFlags& {
  flags_.reset(to_underlying(bit));
  return *this;
}

auto GLClearFlags::Test(GLClearBit bit) const -> bool {
  return flags_.test(to_underlying(bit));
}

}  // namespace graphics_engine::gl_clear_flags

auto graphics_engine::i_gl_clear_flags::CreateIGLClearFlags()
    -> IGLClearFlagsPtr {
  return std::make_unique<gl_clear_flags::GLClearFlags>();
}
