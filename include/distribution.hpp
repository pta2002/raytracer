#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Distribution {
public:
  /**
   * @brief Basic distribution of normals function.
   * @param wh Half-vector
   * @return Normal distribution value for the half-vector
   */
  [[nodiscard]] virtual float d(vec3 wh) const = 0;

  /**
   * @brief Geometry masking function (self-occlusion)
   * @param wo Outgoing vector
   * @param wi Incident vector
   * @return Geometry masking function
   */
  [[nodiscard]] virtual float g(vec3 wo, vec3 wi) const = 0;

  /**
   * @brief Probability derivation function
   * @param wo Outgoing vector
   * @param wh Half-vector
   * @return PDF for the distribution
   */
  [[nodiscard]] virtual float pdf(vec3 wo, vec3 wh) const = 0;

  /**
   * @brief Sample a half-vector for the distribution
   * @param wo Outgoing vector
   * @param u Random numbers in [0,1[
   * @return Sampled half vector for this distribution
   */
  [[nodiscard]] virtual float sample_wh(vec3 wo, vec2 u) const = 0;
};