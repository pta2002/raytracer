#include <light.hpp>
#include <tiny_obj_loader.h>
#include <vector>

std::vector<float> AmbientLight::light(tinyobj::attrib_t attributes,
                                       tinyobj::material_t material) {
  return this->rgb;
}
std::vector<float> Light::light(tinyobj::attrib_t attributes,
                                tinyobj::material_t material) {
  return {};
}
