#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <optional>
#include <string>
#include <tiny_obj_loader.h>
#include <light.hpp>

class Scene {
private:
  void info();

public:
  tinyobj::attrib_t attributes;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::vector<Light> lights;


  /**
   * Loads and creates a scene from a filename
   */
  static std::optional<Scene> load(const std::string &filename,
                                   std::vector<glm::vec3> &vertices,
                                   std::vector<glm::vec3> &vertNormal,
                                   std::vector<glm::vec2> &uvs);
  ~Scene();
};
