#pragma once

#include <glm/glm.hpp>
#include <tiny_obj_loader.h>

using namespace glm;

typedef unsigned char BxDFType;
#define BSDF_SPECULAR 1
#define BSDF_DIFFUSE 2

class Material {
public:
  explicit Material(const tinyobj::material_t &material);

  vec3 specular{};
  vec3 diffuse{};
  vec3 albedo{};
  vec3 emission{};
  vec3 ambient{};
  float shininess;
  float roughness;
  float metallic;
  float ior;

  /**
   *
   * @param wo World origin point
   * @param wi Output: where to bounce the next ray
   * @param pdf Probability density function
   * @param flags Output flags
   * @return Color at the point
   */
  vec3 sampleF(vec3 wo, vec3 &wi, float &pdf, BxDFType &flags) const;
};
