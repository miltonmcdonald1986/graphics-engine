// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_TRIANGLE_H_
#define ENGINE_LIB_TRIANGLE_H_

#include "dll-export.h"

namespace graphics_engine::triangle {

/**
 * @brief Checks whether three given side lengths satisfy the triangle
 * inequality theorem.
 *
 * The triangle inequality states that the sum of any two sides must be greater
 * than the third side. This function returns true if the given sides form a
 * valid triangle, otherwise false.
 *
 * @param side_a First side of the triangle.
 * @param side_b Second side of the triangle.
 * @param side_c Third side of the triangle.
 * @return `true` if the sides satisfy the triangle inequality, `false`
 * otherwise.
 */
DLLEXPORT [[nodiscard]] auto IsValidTriangle(float side_a, float side_b,
                                             float side_c) -> bool;

}  // namespace graphics_engine::triangle

#endif  // ENGINE_LIB_TRIANGLE_H_
