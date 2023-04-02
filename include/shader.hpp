#pragma once

#include "geometry.hpp"
#include "scene.hpp"
#include <glm/glm.hpp>
#include <tiny_obj_loader.h>

using namespace glm;

class Shader {
private:
  const Scene &scene;

public:
  explicit Shader(const Scene &scene) : scene{scene} {};

  virtual vec3 getColor(vec3 ray, optional<vec3> intersection,
                        const Triangle *face) {
    return {0, 0, 0};
  };
};

class AmbientShader : Shader {
private:
  vec3 ambientLight;

public:
  AmbientShader(const Scene &scene, vec3 ambientLight)
      : ambientLight{ambientLight}, Shader(scene){};

  vec3 getColor(vec3 ray, optional<vec3> intersection,
                const Triangle *face) override;
};
