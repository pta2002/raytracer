#pragma once

#include <optional>
#include <string>
#include <tiny_obj_loader.h>

class Scene {
private:
  void info();

public:
  tinyobj::attrib_t attributes;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  /**
   * Loads and creates a scene from a filename
   */
  static std::optional<Scene> load(const std::string &filename);
  ~Scene();
};
