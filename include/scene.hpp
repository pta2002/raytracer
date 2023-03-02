#pragma once

#include <optional>
#include <string>

class Scene {
public:
  /**
   * Loads and creates a scene from a filename
   */
  static std::optional<Scene> load(const std::string &filename);
  ~Scene();
};
