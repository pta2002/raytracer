#pragma once

using namespace std;
using namespace glm;

#include <vector>
#include <array>
#include "material.hpp"

#include "glm.hpp"

class Triangle {
    public:
        Triangle() = default;
        Triangle(int v1, int v2, int v3);

        /**
         * Indices for the three vertices that make up the triangle.
         */
        vec3 vertexIndices;
        /** 
         * Indices for the three normal vectors for each vertex that
         * are needed for interpolated normal calculation in interpolated 
         * shading.
         */  
        vec3 shadingIndices;
        /**  
         * Three dimensional vector describing the normal direction of the
         * triangle for simple shading.
         */
        vec3 geometryNormal;
        /**
         * 
        */
        int material;

        Triangle(vec3 v, vec3 n, vec3 uv, int m) : vertexIndices(v), geometryNormal(n), shadingIndices(uv), material(m) {};
};

class Geometry {
    public:
        Geometry() = default;

        vector<Triangle> triangles;
        Material  material;
};