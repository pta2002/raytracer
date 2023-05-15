#include "material.hpp"
#include <fmt/core.h>

#define arr_to_vec3(X, Y)                                                      \
  X.r = Y[0];                                                                  \
  X.g = Y[1];                                                                  \
  X.b = Y[2];

Material::Material(const tinyobj::material_t &material) {
  fmt::println("Constructing material {}", material.name);
  arr_to_vec3(this->specular, material.specular);
  arr_to_vec3(this->ambient, material.ambient);
  arr_to_vec3(this->diffuse, material.diffuse);
  arr_to_vec3(this->emission, material.emission);
  fmt::println("{} {} {}", diffuse.x, diffuse.y, diffuse.z);
}
