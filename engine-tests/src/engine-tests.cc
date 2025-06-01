// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include <gtest/gtest.h>

#include "graphics-engine/engine.h"

using ::graphics_engine::engine::InitializeEngine;

TEST(EngineTests, InitializeEngine) {
  ASSERT_TRUE(InitializeEngine().has_value());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
