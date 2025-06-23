// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_I_GL_CLEAR_FLAGS_H_
#define ENGINE_LIB_I_GL_CLEAR_FLAGS_H_

#include <memory>

#include "dll-export.h"
#include "gl-types.h"

namespace graphics_engine::gl_clear_flags {

class IGLClearFlags {
 public:
  virtual ~IGLClearFlags() = default;

  virtual auto Set(gl_types::GLClearBit bit) -> IGLClearFlags& = 0;
  virtual auto Reset(gl_types::GLClearBit bit) -> IGLClearFlags& = 0;
  [[nodiscard]] virtual auto Test(gl_types::GLClearBit bit) const -> bool = 0;
};

using IGLClearFlagsPtr = std::unique_ptr<IGLClearFlags>;
DLLEXPORT [[nodiscard]] auto CreateIGLClearFlags() -> IGLClearFlagsPtr;

}  // namespace graphics_engine::i_gl_clear_flags

#endif  // ENGINE_LIB_I_GL_CLEAR_FLAGS_H_
