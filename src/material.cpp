#include "material.hpp"
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

  float tmpRoughness = std::max(roughness, 1e-3f);
  float x = log(tmpRoughness);
  alphax = 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
           0.000640711f * x * x * x * x;
  alphay = alphax;
}

void beckmannSample11(float cosThetaI, float U1, float U2, float *slope_x,
                      float *slope_y) {
  /* Special case (normal incidence) */
  if (cosThetaI > .9999) {
    float r = std::sqrt(-std::log(1.0f - U1));
    float sinPhi = std::sin(2 * M_PIf * U2);
    float cosPhi = std::cos(2 * M_PIf * U2);
    *slope_x = r * cosPhi;
    *slope_y = r * sinPhi;
    return;
  }

  /* The original inversion routine from the paper contained
     discontinuities, which causes issues for QMC integration
     and techniques like Kelemen-style MLT. The following code
     performs a numerical inversion with better behavior */
  float sinThetaI =
      std::sqrt(std::max((float)0, (float)1 - cosThetaI * cosThetaI));
  float tanThetaI = sinThetaI / cosThetaI;
  float cotThetaI = 1 / tanThetaI;

  /* Search interval -- everything is parameterized
     in the Erf() domain */
  float a = -1, c = erf(cotThetaI);
  float sample_x = std::max(U1, (float)1e-6f);

  /* Start with a good initial guess */
  // float b = (1-sample_x) * a + sample_x * c;

  /* We can do better (inverse of an approximation computed in
   * Mathematica) */
  float thetaI = std::acos(cosThetaI);
  float fit = 1 + thetaI * (-0.876f + thetaI * (0.4265f - 0.0594f * thetaI));
  float b = c - (1 + c) * std::pow(1 - sample_x, fit);

  /* Normalization factor for the CDF */
  static const float SQRT_PI_INV = 1.f / std::sqrt(M_PIf);
  float normalization =
      1 / (1 + c + SQRT_PI_INV * tanThetaI * std::exp(-cotThetaI * cotThetaI));

  int it = 0;
  while (++it < 10) {
    /* Bisection criterion -- the oddly-looking
       Boolean expression are intentional to check
       for NaNs at little additional cost */
    if (!(b >= a && b <= c))
      b = 0.5f * (a + c);

    /* Evaluate the CDF and its derivative
       (i.e. the density function) */
    float invErf = erfInv(b);
    float value =
        normalization *
            (1 + b + SQRT_PI_INV * tanThetaI * std::exp(-invErf * invErf)) -
        sample_x;
    float derivative = normalization * (1 - invErf * tanThetaI);

    if (std::abs(value) < 1e-5f)
      break;

    /* Update bisection intervals */
    if (value > 0)
      c = b;
    else
      a = b;

    b -= value / derivative;
  }

  /* Now convert back into a slope value */
  *slope_x = erfInv(b);

  /* Simulate Y component */
  *slope_y = erfInv(2.0f * std::max(U2, (float)1e-6f) - 1.0f);
}

// Helper functions!
vec3 beckmannSample(vec3 wi, float alphax, float alphay, float u1, float u2) {
  vec3 wiStretched = normalize(vec3(alphax * wi.x, wi.y, alphay * wi.z));
  float slopex, slopey;
  beckmannSample11(cosTheta(wiStretched), u1, u2, &slopex, &slopey);

  float tmp = cosPhi(wiStretched) * slopex - sinPhi(wiStretched) * slopey;
  slopey = sinPhi(wiStretched) * slopex + cosPhi(wiStretched) * slopey;
  slopex = tmp;

  slopex = alphax * slopex;
  slopey = alphay * slopey;

  return normalize(vec3(-slopex, 1.f, -slopey));
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
  return distPdf(wo, wh) / (4 * dot(wo, wh));
}

float Material::distPdf(vec3 wo, vec3 wh) const {
  return distribution(wh) * geometry1(wo) * abs(dot(wo, wh)) / absCosTheta(wo);
}

/**
 * @brief Samples a normal vector from a microfacet
 * @param wo outgoing vector IN TANGENT SPACE
 * @param u uniformly sampled random numbers
 * @return random normal vector IN TANGENT SPACE
 */
vec3 Material::sample_wh(const vec3 wo, const vec2 u) const {
  vec3 wh;
  bool flip = wo.y < 0;
  wh = beckmannSample(flip ? -wo : wo, alphax, alphay, u.x, u.y);
  if (flip)
    wh = -wh;
  return wh;
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

  return albedo * distribution(wh) * geometry(wo, wi) * f /
         (4 * cosThetaI * cosThetaO);
}

vec3 Material::sampleF(vec3 wo, vec3 &wi, float &pdf) const {
  const vec2 u = linearRand(vec2{0.f}, vec2{1.f});

  const vec3 wh = sample_wh(wo, u);
  wi = reflect(-wo, wh);
    if (!sameHemisphere(wo, wi)) return vec3{0.f};
  pdf = this->distPdf(wo, wh) / (4.f * dot(wo, wh));

  return f(wo, wi);
}
// For now beckman distribution, TODO use GGX
float Material::distribution(vec3 wh) const {
  const float tan2Theta = tan2theta(wh);
  if (std::isinf(tan2Theta))
    return 0.f;
  float cos4theta = cos2theta(wh) * cos2theta(wh);
  return std::exp(-tan2Theta * (cos2phi(wh) / (alphax * alphax) +
                                sin2phi(wh) / (alphay * alphay))) /
         (M_PIf * alphax * alphay * cos4theta);
}

// I won't pretend like I understand this...
float Material::lambda(vec3 w) const {
  float absTanTheta = std::abs(tanTheta(w));
  if (std::isinf(absTanTheta))
    return 0.f;
  float alpha =
      sqrtf(cos2phi(w) * alphax * alphax + sin2phi(w) * alphay * alphay);
  float a = 1 / (alpha / absTanTheta);
  if (a >= 1.6f)
    return 0;
  return (1.f - 1.259f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a);
}

float Material::geometry(vec3 wo, vec3 wi) const {
  return 1 / (1 + lambda(wo) + lambda(wi));
}
float Material::geometry1(vec3 w) const { return 1 / (1 + lambda(w)); }
