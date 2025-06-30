// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_GL_WRAPPERS_H_
#define ENGINE_LIB_GL_WRAPPERS_H_

#include <bitset>
#include <memory>

#include "dll-export.h"
#include "gl-types.h"
#include "i-gl-clear-flags.h"
#include "types.h"

namespace graphics_engine::gl_wrappers {

DLLEXPORT [[nodiscard]] auto AttachShader(unsigned program, unsigned shader)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto BindBuffer(gl_types::GLBufferTarget target,
                                        unsigned buffer)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto BindVertexArray(unsigned array)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto BufferData(gl_types::GLBufferTarget target,
                                        long long int size, const void* data,
                                        gl_types::GLDataUsagePattern usage)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto Clear(const gl_clear_flags::IGLClearFlags& flags)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto CompileShader(unsigned shader)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto CreateProgram() -> types::Expected<unsigned>;

DLLEXPORT [[nodiscard]] auto CreateShader(gl_types::GLShaderType shader_type)
    -> types::Expected<unsigned>;

DLLEXPORT [[nodiscard]] auto DetachShader(unsigned program, unsigned shader)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto DeleteProgram(unsigned program)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto DeleteShader(unsigned shader_id)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto DrawArrays(gl_types::GLDrawMode mode, int first,
                                        int count) -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto EnableVertexAttribArray(unsigned index)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto GenBuffers(int n, unsigned* buffers)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto GenVertexArrays(int n, unsigned* arrays)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto GetShaderInfoLog(unsigned shader, int max_length,
                                              int* length, char* info_log)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto GetShaderiv(
    unsigned shader, gl_types::GLShaderObjectParameter pname, int* params)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto LinkProgram(unsigned program)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto ShaderSource(unsigned shader, int count,
                                          const char** string,
                                          const int* length)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto UseProgram(unsigned program)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto VertexAttribPointer(unsigned index, int size,
                                                 gl_types::GLDataType type,
                                                 unsigned char normalized,
                                                 int stride,
                                                 const void* pointer)
    -> types::Expected<void>;

}  // namespace graphics_engine::gl_wrappers

#endif  // ENGINE_LIB_GL_WRAPPERS_H_
