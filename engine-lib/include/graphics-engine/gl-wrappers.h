// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_GL_WRAPPERS_H_
#define ENGINE_LIB_GL_WRAPPERS_H_

#include <bitset>
#include <memory>

#include "dll-export.h"
#include "types.h"

namespace graphics_engine::gl_wrappers {

enum class GLBufferTarget : std::uint8_t {
  kArray,
  kCopyRead,
  kCopyWrite,
  kElementArray,
  kPixelPack,
  kPixelUnpack,
  kTexture,
  kTransformFeedback,
  kUniform
};

enum class GLClearBit { kColor, kDepth, kStencil, kNumBits };
constexpr int kExpectedNumClearBits = 3;
static_assert(
    std::to_underlying(GLClearBit::kNumBits) == kExpectedNumClearBits,
    "Fix value of kExpectedNumClearBits to match GLClearBit definition!");

class DLLEXPORT GLClearFlags 
{
 public:
  GLClearFlags();
  ~GLClearFlags();

  GLClearFlags(const GLClearFlags&) = delete;
  GLClearFlags& operator=(const GLClearFlags&) = delete;
  GLClearFlags(GLClearFlags&&) = delete;
  GLClearFlags& operator=(GLClearFlags&&) = delete;
  
  auto Set(GLClearBit bit) -> GLClearFlags&;
  auto Reset(GLClearBit bit) -> GLClearFlags&;
  auto Test(GLClearBit bit) const -> bool;

 private:
  struct Impl;
  Impl* impl_;
};

enum class GLDataType : std::uint8_t {
  kByte,
  kUnsignedByte,
  kShort,
  kUnsignedShort,
  kInt,
  kUnsignedInt,
  kHalfFloat,
  kFloat,
  kDouble,
  kInt_2_10_10_10_Rev,
  kUnsignedInt_2_10_10_10_Rev
};

enum class GLDataUsagePattern : std::uint8_t {
  kStreamDraw,
  kStreamRead,
  kStreamCopy,
  kStaticDraw,
  kStaticRead,
  kStaticCopy,
  kDynamicDraw,
  kDynamicRead,
  kDynamicCopy
};

enum class GLDrawMode : std::uint8_t {
  kPoints,
  kLineStrip,
  kLineLoop,
  kLines,
  kLineStripAdjacency,
  kLinesAdjacency,
  kTriangleStrip,
  kTriangleFan,
  kTriangles,
  kTriangleStripAdjacency,
  kTrianglesAdjacency
};

DLLEXPORT [[nodiscard]] auto BindBuffer(GLBufferTarget target,
                                        unsigned int buffer)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto BindVertexArray(unsigned int array)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto BufferData(GLBufferTarget target,
                                        long long int size, const void* data,
                                        GLDataUsagePattern usage)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto Clear(const GLClearFlags& flags) -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto DrawArrays(GLDrawMode mode, int first, int count)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto EnableVertexAttribArray(unsigned int index)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto GenBuffers(int n, unsigned int* buffers)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto GenVertexArrays(int n, unsigned int* arrays)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto UseProgram(unsigned int program)
    -> types::Expected<void>;

DLLEXPORT [[nodiscard]] auto VertexAttribPointer(
    unsigned int index, int size, GLDataType type, unsigned char normalized,
    int stride, const void* pointer) -> types::Expected<void>;

}  // namespace graphics_engine::gl_wrappers

#endif  // ENGINE_LIB_GL_WRAPPERS_H_
