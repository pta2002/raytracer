#include "distributions/beckmann.hpp"
BeckmannDistribution::BeckmannDistribution(float alphax, float alphay)
    : alphax(alphax), alphay(alphay) {}

BeckmannDistribution::BeckmannDistribution(float roughness) {
  // Conversion function taken from pbrt
  float tmpRoughness = std::max(roughness, 1e-3f);
  float x = log(tmpRoughness);
  alphax = 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
           0.000640711f * x * x * x * x;
  alphay = alphax;
}
