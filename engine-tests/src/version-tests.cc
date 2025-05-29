#include <gtest/gtest.h>

#include "graphics-engine/version.h"

using ::graphics_engine::GetEngineLibVersion;

TEST(GraphicsEngineTests, GetEngineLibVersion) { 
  EXPECT_EQ(GetEngineLibVersion(), "1.0.0");
}
