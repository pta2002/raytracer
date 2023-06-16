#include "material.hpp"
#include "distributions/ggx.hpp"
#include "fmt/core.h"
#include "helpers.hpp"

#include <glm/gtc/random.hpp>

#define arr_to_vec3(X, Y)                                                      \
  X.r = Y[0];                                                                  \
  X.g = Y[1];                                                                  \
  X.b = Y[2];

Material::Material(const tinyobj::material_t &material) {
  arr_to_vec3(this->specular, material.specular);
  arr_to_vec3(this->ambient, material.ambient);
  arr_to_vec3(this->diffuse, material.diffuse);
  arr_to_vec3(this->emission, material.emission);

  this->shininess = material.shininess;

  // Now it's time to experience blender's horrors at trying to convert tens of
  // parameters into a format made in the 90s...
  //
  // From
  // https://github.com/blender/blender/blob/main/source/blender/io/wavefront_obj/importer/obj_import_mtl.cc#L247
  // Blender encodes the PBR roughness into the shininess property, so we need
  // to reverse it.
  this->roughness =
      1.0f -
      sqrt(std::max(0.0f, std::min(1000.0f, material.shininess)) / 1000.0f);

  if (material.illum == 3) {
    // Metallic is the average of the ambient color
    this->metallic = (ambient.r + ambient.b + ambient.g) / 3;
  } else {
    this->metallic = 0.0f;
  }

  this->albedo = diffuse;

  this->ior = material.ior;

  this->f0 = vec3{(1.0f - ior) / (1.0f + ior)};
  this->f0 = f0 * f0;
  this->f0 = mix(f0, albedo, metallic);

  dist = std::make_shared<GGXDistribution>(roughness);
}

vec3 fresnelSchlick(float cosTheta, vec3 f0) {
  return f0 + (vec3{1.0f} - f0) * powf(1 - cosTheta, 5);
}

/**
 * @param normal the normal of the surface
 * @param wm the normal of the microfacet (half-vector)
 * @return PDF of the microfacet material
 */
float Material::pdf(vec3 wo, vec3 wi) const {
  if (!sameHemisphere(wo, wi))
    return 0;
  vec3 wh = normalize(wo + wi);
  return dist->pdf(wo, wh) / (4 * dot(wo, wh));
}

/**
 * Executes the BRDF
 * @param wo
 * @param wi
 * @return
 */
vec3 Material::f(vec3 wo, vec3 wi) const {
  const float cosThetaO = absCosTheta(wo), cosThetaI = absCosTheta(wi);
  const vec3 wh = normalize(wi + wo);
  const vec3 f = fresnelSchlick(dot(wi, wh), f0);

  return albedo * dist->d(wh) * dist->g(wo, wi) * f /
         (4 * cosThetaI * cosThetaO);
}

vec3 Material::sampleF(vec3 wo, vec3 &wi, float &pdf) const {
  const vec2 u = linearRand(vec2{0.f}, vec2{1.f});

  const vec3 wh = dist->sample_wh(wo, u);
  wi = 2 * abs(dot(wo, wh)) * wh - wo;
  if (!sameHemisphere(wo, wi))
    return vec3{0.f};
  pdf = this->dist->pdf(wo, wh) / (4.f * dot(wo, wh));

  return f(wo, wi);
}
