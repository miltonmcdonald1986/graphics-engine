// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "gtest/gtest.h"

#include "graphics-engine/triangle.h"

using ::graphics_engine::triangle::IsValidTriangle;

TEST(TriangleTests, IsValidTriangle) {
  ASSERT_TRUE(IsValidTriangle(3.F, 4.F, 5.F));
  ASSERT_FALSE(IsValidTriangle(-3.F, 4.F, 5.F));
  ASSERT_FALSE(IsValidTriangle(3.F, -4.F, 5.F));
  ASSERT_FALSE(IsValidTriangle(3.F, 4.F, -5.F));
  ASSERT_FALSE(IsValidTriangle(2.F, 3.F, 6.F));
}
