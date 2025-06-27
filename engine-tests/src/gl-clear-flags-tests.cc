// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <graphics-engine/i-gl-clear-flags.h>

#include "gtest/gtest.h"

using enum graphics_engine::gl_types::GLClearBit;

using graphics_engine::gl_clear_flags::CreateIGLClearFlags;
using graphics_engine::gl_types::GLClearBit;

TEST(GLClearFlagsTest, CanSetAndTestFlags) {
  auto clear_flags = CreateIGLClearFlags();

  // flags should be false by default.
  ASSERT_FALSE(clear_flags->Test(kColor));
  ASSERT_FALSE(clear_flags->Test(kDepth));
  ASSERT_FALSE(clear_flags->Test(kStencil));

  // The counter value should not work.
  ASSERT_THROW((void)clear_flags->Test(kNumBits), std::out_of_range);

  // Other garbage inputs should also not work.
  ASSERT_THROW((void)clear_flags->Test(static_cast<GLClearBit>(42)),
               std::out_of_range);

  // Set and reset flags
  clear_flags->Set(kColor);
  ASSERT_TRUE(clear_flags->Test(kColor));
  clear_flags->Reset(kColor);
  ASSERT_FALSE(clear_flags->Test(kColor));

  clear_flags->Set(kDepth);
  ASSERT_TRUE(clear_flags->Test(kDepth));
  clear_flags->Reset(kDepth);
  ASSERT_FALSE(clear_flags->Test(kDepth));

  clear_flags->Set(kStencil);
  ASSERT_TRUE(clear_flags->Test(kStencil));
  clear_flags->Reset(kStencil);
  ASSERT_FALSE(clear_flags->Test(kStencil));

  ASSERT_THROW(clear_flags->Set(kNumBits), std::out_of_range);
  ASSERT_THROW(clear_flags->Reset(kNumBits), std::out_of_range);
  ASSERT_THROW(clear_flags->Set(static_cast<GLClearBit>(42)), std::out_of_range);
  ASSERT_THROW(clear_flags->Reset(static_cast<GLClearBit>(42)), std::out_of_range);
}
