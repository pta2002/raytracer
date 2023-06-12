#pragma once

#include "distribution.hpp"

class BeckmannDistribution : public Distribution {
public:
  float alphax, alphay;

  BeckmannDistribution(float alphax, float alphay);
  ~BeckmannDistribution() override = default;

  /**
   * @brief Initialize a beckmann distribution with a roughness value, which
   * will be converted
   * @param roughness This value will be converted to alphax and alphay
   */
  explicit BeckmannDistribution(float roughness);

  vec3 sample_wh(vec3 wo, vec2 u) const override;
  float d(vec3 wh) const override;
  float g(vec3 wo, vec3 wi) const override;
  float g1(vec3 w) const override;
  float pdf(vec3 wo, vec3 wh) const override;

private:
  float lambda(vec3 w) const;
};