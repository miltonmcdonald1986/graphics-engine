// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_SHADER_H_
#define ENGINE_LIB_SHADER_H_

#include <vector>

#include "glad/glad.h"
#include "graphics-engine/gl-wrappers.h"
#include "graphics-engine/i-shader.h"
#include "graphics-engine/types.h"

namespace graphics_engine::shader {

class Shader : public IShader {
 public:
  Shader() = default;
  ~Shader() override = default;

  [[nodiscard]] auto GetProgramId() const -> unsigned int override;

  [[nodiscard]] auto Initialize(const types::ShaderSourceMap& sources)
      -> types::Expected<void>;

 private:
  GLuint program_id_{};
};

}  // namespace graphics_engine::shader

#endif  // ENGINE_LIB_SHADER_H_
