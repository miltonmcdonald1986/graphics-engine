#ifndef ENGINE_LIB_VERSION_H_
#define ENGINE_LIB_VERSION_H_

#include <string_view>

#include "dll-export.h"

namespace graphics_engine {

DLLEXPORT [[nodiscard]] auto GetEngineLibVersion() -> std::string_view;

}  // namespace graphics_engine

#endif  // ENGINE_LIB_VERSION_H_
