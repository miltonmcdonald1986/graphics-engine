// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_SHADER_H_
#define ENGINE_LIB_SHADER_H_

#include "dll-export.h"
#include "types.h"

namespace graphics_engine::shader {

enum class ShaderType : std::uint8_t {
  kFragment,
  kGeometry,
  kVertex,
  kNumShaderTypes
};

DLLEXPORT [[nodiscard]] auto CompileShader(unsigned int shader_id,
                                           const std::string& source_code)
    -> ::graphics_engine::types::Expected<void>;

DLLEXPORT [[nodiscard]] auto CreateShader(ShaderType type)
    -> ::graphics_engine::types::Expected<unsigned int>;

}  // namespace graphics_engine::shader

#endif  // ENGINE_LIB_SHADER_H_
