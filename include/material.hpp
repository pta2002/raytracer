#pragma once

#include <glm/glm.hpp>
#include <tiny_obj_loader.h>

using namespace glm;

typedef unsigned char BxDFType;
#define BSDF_SPECULAR 1
#define BSDF_DIFFUSE 2

class Material {
private:
  float alphax, alphay;
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

  vec3 f0;

  /**
   * @brief chooses a direction according to a distribution from its scattering function
   * @param wo World origin point
   * @param wi Output: where to bounce the next ray
   * @param pdf Output: Probability density function
   * @param flags Output: flags
   * @return Color at the point
   */
  [[nodiscard]] vec3 sampleF(vec3 wo, vec3 &wi, float &pdf) const;
  [[nodiscard]] vec3 sample_wh(const vec3 wo, const vec2 u) const;
  [[nodiscard]] float pdf(vec3 normal, vec3 half) const;
  [[nodiscard]] vec3 f(vec3 wo, vec3 wi) const;
  [[nodiscard]] float distribution(vec3 wh) const;
  [[nodiscard]] float geometry(vec3 wo, vec3 wi) const;
  float lambda(vec3 w) const;
  float geometry1(vec3 w) const;
  float distPdf(vec3 wo, vec3 wh) const;
};
