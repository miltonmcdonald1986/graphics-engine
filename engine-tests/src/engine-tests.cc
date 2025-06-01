// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <gtest/gtest.h>

#include "graphics-engine/engine.h"

using ::graphics_engine::engine::InitializeEngine;

TEST(EngineTests, InitializeEngineNoContext) {
  auto result = InitializeEngine();
  ASSERT_FALSE(result.has_value());
  ASSERT_STREQ(result.error().category().name(), "graphics_engine::error");
  ASSERT_EQ(result.error().value(), 1);
  ASSERT_EQ(result.error().message(), "Engine Initialization failed.");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
