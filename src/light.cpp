#include <tiny_obj_loader.h>
#include <light.hpp>
#include <vector>

std::vector<float> AmbientLight::light(tinyobj::attrib_t attributes, tinyobj::material_t material) {
    return this->rgb;
}