#pragma once

#include "distribution.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <tiny_obj_loader.h>

using namespace glm;

class Material {
private:
  // No, this does _not_ need to be a shared pointer, but seemed to be the only
  // way of pleasing the C++ compiler gods...
  std::shared_ptr<Distribution> dist;

public:
  explicit Material(const tinyobj::material_t &material);
  ~Material() = default;

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
   * @brief chooses a direction according to a distribution from its scattering
   * function
   * @param wo World origin point
   * @param wi Output: where to bounce the next ray
   * @param pdf Output: Probability density function
   * @param flags Output: flags
   * @return Color at the point
   */
  [[nodiscard]] vec3 sampleF(vec3 wo, vec3 &wi, float &pdf) const;
  [[nodiscard]] float pdf(vec3 normal, vec3 half) const;
  [[nodiscard]] vec3 f(vec3 wo, vec3 wi) const;
};
