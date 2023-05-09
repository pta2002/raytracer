#include "material.hpp"

#define arr_to_vec3(X, Y)                                                      \
  X.r = Y[0];                                                                  \
  X.g = Y[1];                                                                  \
  X.b = Y[2];

Material::Material(const tinyobj::material_t &material) {
  arr_to_vec3(this->specular, material.specular);

  this->diffuse.r = material.diffuse[0];
  this->diffuse.g = material.diffuse[1];
  this->diffuse.b = material.diffuse[2];

  this->emission.r = material.emission[0];
  this->emission.g = material.emission[1];
  this->emission.b = material.emission[2];
}
