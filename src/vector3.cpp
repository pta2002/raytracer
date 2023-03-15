#include "vector3.hpp"
#include <math.h>

using namespace std;

vec3::vec3(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

vec3::vec3(float _x, float _z) {
    x = _x;
    y =  0;
    z = _z;
}

vec3 vec3::fromSpherical(float alpha, float beta, float radius) {
    float x = cos(beta) * sin(alpha) * radius;
    float y = sin(beta) * radius;
    float z = cos(beta) * cos(alpha) * radius;

    return vec3(x, y, z);
}

void vec3::applyVector(vec3 v) {
    x += v.x;
    y += v.y;
    z += v.z;
}