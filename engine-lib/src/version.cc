#include "graphics-engine/version.h"

constexpr int kEngineLibVersionMajor = 1;
constexpr int kEngineLibVersionMinor = 0;
constexpr int kEngineLinVersionPatch = 0;

constexpr std::string_view kEngineLibVersionString = "1.0.0";

namespace graphics_engine {

[[nodiscard]] auto GetEngineLibVersion() -> std::string_view {
  return kEngineLibVersionString;
}

}  // namespace graphics_engine
