// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#include "gtest/gtest.h"

#include "graphics-engine/version.h"

using ::graphics_engine::version::GetEngineLibVersion;

TEST(VersionTests, GetEngineLibVersion) { 
  ASSERT_EQ(GetEngineLibVersion(), "1.0.0");
}
