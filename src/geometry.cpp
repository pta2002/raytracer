#include "geometry.hpp"
#include <glm/glm.hpp>

array<vec3, 3> Triangle::getVertices(const tinyobj::attrib_t &attrib) {
  return {
      vec3{attrib.vertices[vertexIndices[0] * 3],
           attrib.vertices[vertexIndices[0] * 3 + 1],
           attrib.vertices[vertexIndices[0] * 3 + 2]},
      vec3{attrib.vertices[vertexIndices[1] * 3],
           attrib.vertices[vertexIndices[1] * 3 + 1],
           attrib.vertices[vertexIndices[1] * 3 + 2]},
      vec3{attrib.vertices[vertexIndices[2] * 3],
           attrib.vertices[vertexIndices[2] * 3 + 1],
           attrib.vertices[vertexIndices[2] * 3 + 2]},
  };
}

bool sameSide(vec3 A, vec3 B, vec3 C, vec3 D, vec3 p) {
  vec3 normal = cross(B - A, C - A);
  double dotD = dot(normal, D - A);
  double dotP = dot(normal, p - A);
  return signbit(dotD) == signbit(dotP);
}

bool Triangle::intersects(const tinyobj::attrib_t &attrib, vec3 ray, vec3 origin) {
  auto vertices = getVertices(attrib);

  return sameSide(vertices[0], vertices[1], vertices[2], origin, origin+ray)
  && sameSide(vertices[1], vertices[2], origin, vertices[0], origin + ray)
  && sameSide(vertices[2], origin, vertices[0], vertices[1], origin + ray)
  && sameSide(origin, vertices[0], vertices[1], vertices[2], origin + ray);
}
