#pragma once

#include "camera.hpp"
#include "geometry.hpp"
#include "image.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <light.hpp>
#include <memory>
#include <optional>
#include <string>
#include <tiny_obj_loader.h>

using namespace glm;

struct Intersection {
  vec3 ray;
  optional<vec3> pos;
  const Triangle *face;
};

class Scene {
private:
  std::vector<Object> objects;

  const Camera *camera = nullptr;

public:
  tinyobj::attrib_t attributes;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<Material> materials;
  std::vector<Light> lights;

  /**
   * Loads and creates a scene from a filename
   */
  static std::optional<Scene> load(const std::string &filename);
  ~Scene();

  void setCamera(const Camera &camera);

  [[nodiscard]] Intersection castRay(vec3 origin, vec3 direction) const;

  Image render();
};

class SceneDef {
public:
  std::string modelFile, outputFile;
  uint32_t width, height;
  shared_ptr<Camera> camera;

  explicit SceneDef(const std::string &filename);

  optional<Scene> getScene();
};
