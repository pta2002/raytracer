#pragma once

#include <glm/glm.hpp>
#include <tiny_obj_loader.h>
#include <vector>

using namespace glm;

enum LightType { AMBIENT, POINT, UNDEFINED };

class Light {
public:
  Light() = default;
  virtual ~Light() = default;

  vec3 rgb{};

  [[nodiscard]] virtual LightType lightType() const;
};

class AmbientLight : public Light {
public:
  vec3 color;

  explicit AmbientLight(vec3 color);

  [[nodiscard]] LightType lightType() const override;
};

class PointLight : public Light {
public:
  vec3 position;
  vec3 color;

  PointLight(vec3 pos, vec3 color);
  [[nodiscard]] LightType lightType() const override;
};
