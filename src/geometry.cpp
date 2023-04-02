#include "geometry.hpp"
#include <glm/glm.hpp>
#include <optional>

Triangle::Triangle(array<vec3, 3> vertices, optional<array<vec3, 3>> normals,
                   vec3 texCoords, const tinyobj::material_t *material)
    : vertices{vertices}, normals{normals}, texcoords{texCoords},
      material{material} {
  this->planeNormal =
      cross(vertices[1] - vertices[0], vertices[2] - vertices[0]);
}

optional<vec3> Triangle::intersects(vec3 ray, vec3 origin) {
  vec3 E1 = vertices[1] - vertices[0];
  vec3 E2 = vertices[2] - vertices[0];
  float det = -dot(ray, planeNormal);
  float invDet = 1.0f / det;
  vec3 AO = origin - vertices[0];
  vec3 DAO = cross(AO, ray);
  float u = dot(E2, DAO) * invDet;
  float v = -dot(E1, DAO) * invDet;
  float t = dot(AO, planeNormal) * invDet;

  if (det >= 1e-6 && t >= 0 && u > 0 && v > 0 && u + v <= 1) {
    return {vec3(origin + t * ray)};
  }
  return {};
}
