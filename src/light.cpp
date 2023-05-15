#include <light.hpp>
#include <tiny_obj_loader.h>
#include <vector>

LightType Light::lightType() const { return UNDEFINED; }

// Ambinet
AmbientLight::AmbientLight(vec3 color) : color(color) {}
LightType AmbientLight::lightType() const { return AMBIENT; }

// Point
PointLight::PointLight(vec3 pos, vec3 color) : position(pos), color(color) {}
LightType PointLight::lightType() const { return POINT; }
