// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_GL_WRAPPERS_H_
#define ENGINE_LIB_GL_WRAPPERS_H_

#include "dll-export.h"
#include "types.h"

namespace graphics_engine::gl_wrappers {

DLLEXPORT [[nodiscard]] auto BindBuffer(
    ::graphics_engine::types::GLBufferTarget target, unsigned int buffer)
    -> ::graphics_engine::types::Expected<void>;

DLLEXPORT [[nodiscard]] auto BindVertexArray(unsigned int array)
    -> ::graphics_engine::types::Expected<void>;

DLLEXPORT [[nodiscard]] auto BufferData(
    ::graphics_engine::types::GLBufferTarget target, long long int size,
    const void* data, ::graphics_engine::types::GLDataUsagePattern usage)
    -> ::graphics_engine::types::Expected<void>;

DLLEXPORT [[nodiscard]] auto GenBuffers(int n, unsigned int* buffers)
    -> ::graphics_engine::types::Expected<void>;

DLLEXPORT [[nodiscard]] auto GenVertexArrays(int n, unsigned int* arrays)
    -> ::graphics_engine::types::Expected<void>;

}  // namespace graphics_engine::gl_wrappers

#endif  // ENGINE_LIB_GL_WRAPPERS_H_
