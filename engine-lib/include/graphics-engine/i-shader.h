// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_I_SHADER_H_
#define ENGINE_LIB_I_SHADER_H_

#include <memory>

#include "dll-export.h"
#include "types.h"

namespace graphics_engine::shader {

class IShader {
 public:
  virtual ~IShader() = default;

  [[nodiscard]] virtual auto GetProgramId() const -> unsigned int = 0;
};

using IShaderPtr = std::unique_ptr<IShader>;
DLLEXPORT [[nodiscard]] auto CreateIShader(
    const types::ShaderSourceMap& sources) -> IShaderPtr;

}  // namespace graphics_engine::i_shader

#endif  // ENGINE_LIB_I_SHADER_H_
