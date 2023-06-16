#include "bsdf.hpp"
#include "glm/geometric.hpp"

using namespace glm;

BSDF::BSDF(const Intersection &intersection) {
  ns = *intersection.shadingNormal;
  ss = normalize(intersection.face->vertices[1] -
                 intersection.face->vertices[0]);
  ts = cross(ns, ss);

  if (ns != intersection.face->planeNormal) {
    ss = -ss;
    ts = -ts;
  }
  material = intersection.face->material;
  geometricNormal = *intersection.geometricNormal;
}

vec3 BSDF::worldToLocal(vec3 v) const {
  return vec3{dot(v, ss), dot(v, ns), dot(v, ts)};
}

vec3 BSDF::localToWorld(vec3 v) const {
  return vec3{ss.x * v.x + ts.x * v.y + ns.x * v.z,
              ss.y * v.x + ts.y * v.y + ns.y * v.z,
              ss.z * v.x + ts.z * v.y + ns.z * v.z};
}

vec3 BSDF::f(vec3 woWorld, vec3 wiWorld) const {
  vec3 wi = worldToLocal(wiWorld), wo = worldToLocal(woWorld);
  return material->f(wo, wi);
}

vec3 BSDF::sampleF(vec3 woWorld, vec3 &wiWorld, float &pdf) const {
  vec3 wi, wo = worldToLocal(woWorld);

  vec3 f = material->sampleF(wo, wi, pdf);
  if (pdf == 0)
    return vec3{0.f};

  wiWorld = localToWorld(wi);

  if (dot(wiWorld, ns) < 0)
    wiWorld = -wiWorld;

  return f;
}