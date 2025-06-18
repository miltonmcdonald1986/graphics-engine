// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_SHADER_H_
#define ENGINE_LIB_SHADER_H_

#include <vector>

#include "dll-export.h"
#include "gl-wrappers.h"
#include "types.h"

namespace graphics_engine::shader {

/// @brief Replaces the source code in a shader object and compiles the shader
/// object.
/// @param shader_id specifies the shader object to be compiled.
/// @param source_code specifies a string containing the source code to be
/// loaded into the shader.
/// @return void on success, error on failure.
DLLEXPORT [[nodiscard]] auto CompileShader(unsigned int shader_id,
                                           const std::string& source_code)
    -> ::graphics_engine::types::Expected<void>;

/// @brief Creates a shader object of the given type, and compiles it using the
/// given source code string.
/// @param type specifies the type of shader to be created.
/// @param source_code specifies a string containing the source code to be
/// loaded into the shader.
/// @return an identifier for the shader on success, error on failure.
DLLEXPORT [[nodiscard]] auto CreateAndCompileShader(
    gl_wrappers::GLShaderType type, const std::string& source_code)
    -> types::Expected<unsigned int>;

/// @brief Create a shader program, attach the given shaders, and link the
/// program.
/// @param shader_ids identifiers for the shaders to be attached.
/// @return program id on success, error on failure.
DLLEXPORT [[nodiscard]] auto CreateAndLinkShaderProgram(
    const std::vector<unsigned int>& shader_ids)
    -> ::graphics_engine::types::Expected<unsigned int>;

/// @brief Wrapper around glCreateProgram
/// @return a non-zero value by which the created program can be referenced.
DLLEXPORT [[nodiscard]] auto CreateProgram()
    -> ::graphics_engine::types::Expected<unsigned int>;

/// @brief Wrapper around glDeleteShader with error handling.
/// @param shader_id Specifies the shader object to be deleted.
/// @return void on success, error on failure.
DLLEXPORT [[nodiscard]] auto DeleteShader(unsigned int shader_id)
    -> ::graphics_engine::types::Expected<void>;

/// @brief Wrapper around glShaderSource with error handling.
/// @param shader Specifies the handle of the shader object whose source code is
/// to be replaced.
/// @param count Specifies the number of elements in the string and length
/// arrays.
/// @param string Specifies an array of pointers to strings containing the
/// source code to be loaded into the shader.
/// @param length Specifies an array of string lengths.
/// @return void on success, error on failure.
DLLEXPORT [[nodiscard]] auto ShaderSource(unsigned int shader, int count,
                                          const char* const* string,
                                          const int* length)
    -> ::graphics_engine::types::Expected<void>;

}  // namespace graphics_engine::shader

#endif  // ENGINE_LIB_SHADER_H_
