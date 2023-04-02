#pragma once

#include "material.hpp"
#include <array>
#include <tiny_obj_loader.h>
#include <vector>
#include <optional>

#include <glm/vec3.hpp>

using namespace std;
using namespace glm;

class Triangle {
public:
    Triangle() = default;
    Triangle(array<vec3, 3> vertices, optional<array<vec3, 3>> normals,
             vec3 texCoords, const tinyobj::material_t *material);

  /**
   * The three vertices that make up triangle.
   */
  array<vec3, 3> vertices;

  vec3 planeNormal{};

  /**
   * The three normal vectors for each vertex that
   * are needed for interpolated normal calculation in interpolated
   * shading.
   */
  std::optional<array<vec3, 3>> normals;

  /**
   * TODO figure this out, but AFAIK it's an array of vec2's, describing the texture coordinates for each corner
   */
  vec3 texcoords;
  /**
   *
   */
  const tinyobj::material_t *material;

  /**
   * Checks if a ray intersects the triangle
   * @param ray The ray vector
   * @param origin The origin point of the ray
   * @return Boolean specifying whether an intersection was detected
   */
  optional<vec3> intersects(vec3 ray, vec3 origin);
};

class Geometry {
public:
  Geometry() = default;

  vector<Triangle> triangles;
  Material material;
};