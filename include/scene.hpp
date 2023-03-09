#pragma once

#include <optional>
#include <string>

class Scene {
public:

  static attrib_t attributes;
  
  static std::vector<shape_t> shapes;

  static std::vector<material_t> materials;
  /**
   * Loads and creates a scene from a filename
   */
  static std::optional<Scene> load(const std::string &filename, const std::string mtlDir);
  ~Scene();
};
