#pragma once

using namespace std;

#include <vector>
#include "material.hpp"

class Triangle {
    public:
        Triangle() = default;
        Triangle(int v1, int v2, int v3);

        /**
         * Indices for the three vertices that make up the triangle.
         */
        int vertexIndices[3];
        /** 
         * Indices for the three normal vectors for each vertex that
         * are needed for interpolated normal calculation in interpolated 
         * shading.
         */  
        int shadingIndices[3];
        /**  
         * Three dimensional vector describing the normal direction of the
         * triangle for simple shading.
         */
        float geometryNormal[3];
        /**
         * 
        */
};

class Geometry {
    public:
        Geometry() = default;

        vector<Triangle> triangles;
        Material  material;
};