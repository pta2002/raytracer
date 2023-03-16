#pragma once

#include "material.hpp"
#include <array>
#include <vector>

#include <glm/vec3.hpp>

using namespace std;
using namespace glm;

class Triangle {
public:
  Triangle() = default;
  Triangle(array<int, 3> vertices, array<int, 3> normals,
           array<int, 3> texcoords, int material)
      : vertexIndices{vertices}, normalIndices{normals},
        texcoordIndices{texcoords}, material{material} {};

  /**
   * Indices for the three vertices that make up the triangle.
   */
  array<int, 3> vertexIndices;
  /**
   * Indices for the three normal vectors for each vertex that
   * are needed for interpolated normal calculation in interpolated
   * shading.
   */
  array<int, 3> normalIndices;
  /**
   * Three dimensional vector describing the normal direction of the
   * triangle for simple shading.
   */
  array<int, 3> texcoordIndices;
  /**
   *
   */
  int material;
};

class Geometry {
public:
  Geometry() = default;

  vector<Triangle> triangles;
  Material material;
};