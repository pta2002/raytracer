#include "shader.hpp"
#include <fmt/core.h>

vec3 AmbientShader::getColor(vec3 ray, optional<vec3> intersection,
                             const Triangle *face) {
  vec3 color{0};
  if (face) {
    vec3 curLight = ambientLight * dot(-ray, normalize(face->planeNormal));
    if (face->material) {
      color.r += face->material->diffuse[0] * curLight.r;
      color.g += face->material->diffuse[1] * curLight.g;
      color.b += face->material->diffuse[2] * curLight.b;
    }
  }
  return color;
}
