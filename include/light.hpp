#pragma once

#include <glm/glm.hpp>
#include <tiny_obj_loader.h>
#include <vector>

using namespace glm;

enum LightType { AMBIENT, POINT, UNDEFINED };

class Light {
public:
  Light() = default;
  ~Light() = default;

  vec3 rgb{};

  /**
   * Returns the color of the point as affected by this light source
   */
  virtual vec3 light(tinyobj::attrib_t attributes,
                     tinyobj::material_t material);
  virtual vec3 light();
  virtual LightType lightType();
};

class AmbientLight : public Light {
public:
  AmbientLight() = default;
  ~AmbientLight() = default;

  vec3 light(tinyobj::attrib_t attributes,
             tinyobj::material_t material) override;
  vec3 light() override;
  inline LightType lightType() override { return AMBIENT; };
};

class PointLight : public Light {
public:
  vec3 position;
  vec3 color;

  PointLight(vec3 pos, vec3 color);
  inline LightType lightType() override { return POINT; };
};