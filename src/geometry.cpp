#include "geometry.hpp"

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
