#include "material.hpp"

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
}

vec3 Material::sampleF(vec3 wo, vec3 &wi, float &pdf, BxDFType &flags) const {
  // TODO
  return {};
}
