#pragma once

#include "distribution.hpp"

class BeckmannDistribution : public Distribution {
public:
  float alphax, alphay;

  BeckmannDistribution(float alphax, float alphay);

  /**
   * @brief Initialize a beckmann distribution with a roughness value, which will be converted
   * @param roughness This value will be converted to alphax and alphay
   */
  explicit BeckmannDistribution(float roughness);
};