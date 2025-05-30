// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "graphics-engine/triangle.h"

namespace graphics_engine::triangle {

auto IsValidTriangle(float side_a, float side_b, float side_c) -> bool {
  return side_a >= 0.F && side_b >= 0.F && side_c >= 0.F &&
         (side_a + side_b > side_c) && (side_a + side_c > side_b) &&
         (side_b + side_c > side_a);
}

}  // namespace graphics_engine::triangle
