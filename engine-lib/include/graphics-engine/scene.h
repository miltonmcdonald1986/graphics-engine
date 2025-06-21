// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_SCENE_H_
#define ENGINE_LIB_SCENE_H_

#include <memory>

#include "types.h"

namespace graphics_engine::scene {

class Scene {
 public:
  virtual ~Scene() = default;
  [[nodiscard]] virtual auto Render() const -> types::Expected<void> = 0;
};

using ScenePtr = std::unique_ptr<Scene>;

}  // namespace graphics_engine::scene

#endif  // ENGINE_LIB_SCENE_H_
