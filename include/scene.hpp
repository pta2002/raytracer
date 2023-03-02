#pragma once

#include <optional>
#include <string>

class Scene {
public:
  static std::optional<Scene> load(const std::string &filename);
  ~Scene();
};
