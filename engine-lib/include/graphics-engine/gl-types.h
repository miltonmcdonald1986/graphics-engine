// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_GL_TYPES_H_
#define ENGINE_LIB_GL_TYPES_H_

#include <cstdint>

namespace graphics_engine::gl_types {

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

enum class GLClearBit : std::uint8_t { kColor, kDepth, kStencil, kNumBits };

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

enum class GLShaderType : std::uint8_t { kFragment, kGeometry, kVertex };

}  // namespace graphics_engine::gl_types

#endif  // ENGINE_LIB_GL_TYPES_H_
