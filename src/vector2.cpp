#include "vector2.hpp"
#include <math.h>

using namespace std;

vec2::vec2(float _x, float _y) {
    x = _x;
    y = _y;
}

void vec2::applyVector(vec2 v) {
    x += v.x;
    y += v.y;
}