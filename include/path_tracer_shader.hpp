#pragma once

#include "shader.hpp"

class PathTracerShader : public Shader {
private:
  vec3 background;
  vec3 directLighting(const Intersection &isect, const Material &material);
  vec3 specularReflection(const Intersection &isect, const Material &material);

public:
  explicit PathTracerShader(const Scene &scene) : Shader(scene){};
  vec3 getColor(const Intersection &intersection) override;
};