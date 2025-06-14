// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_GL_WRAPPERS_H_
#define ENGINE_LIB_GL_WRAPPERS_H_

#include "dll-export.h"
#include "types.h"

namespace graphics_engine::gl_wrappers {

/// <summary>
/// Bind a named buffer object.
/// </summary>
/// <param name="target">Specifies the target to which the buffer object
/// isbound.</param> <param name="buffer">Specifies the name of a buffer
/// object.</param> <returns>void on success, error on failure.</returns>
DLLEXPORT [[nodiscard]] auto BindBuffer(
    ::graphics_engine::types::GLBufferTarget target, unsigned int buffer)
    -> ::graphics_engine::types::Expected<void>;

/// <summary>
/// Bind a vertex array object.
/// </summary>
/// <param name="array">Specifies the name of the vertex array to bind.</param>
/// <returns>void on success, error on failure.</returns>
DLLEXPORT [[nodiscard]] auto BindVertexArray(unsigned int array)
    -> ::graphics_engine::types::Expected<void>;

/// <summary>
/// Generate buffer object names.
/// </summary>
/// <param name="n">Specifies the number of buffer object names to be
/// generated.</param> <param name="buffers">Specifies an array in which the
/// generated buffer object names are stored.</param> <returns>void on success,
/// error on failure.</returns>
DLLEXPORT [[nodiscard]] auto GenBuffers(int n, unsigned int* buffers)
    -> ::graphics_engine::types::Expected<void>;

/// <summary>
/// Generate vertex array object names.
/// </summary>
/// <param name="n">Specifies the number of vertex array object names to
/// generate.</param>
/// <param name="arrays">Specifies an array in which the generated vertex array
/// object names are stored.</param>
/// <returns>void on success, error on failure.</returns>
DLLEXPORT [[nodiscard]] auto GenVertexArrays(int n, unsigned int* arrays)
    -> ::graphics_engine::types::Expected<void>;

}  // namespace graphics_engine::gl_wrappers

#endif  // ENGINE_LIB_GL_WRAPPERS_H_
