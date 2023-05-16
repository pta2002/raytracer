#include "shader.hpp"
#include "light.hpp"
#include <fmt/core.h>
#include <glm/gtc/random.hpp>

vec3 AmbientShader::getColor(const Intersection &intersection) {
  vec3 color{0};
  if (intersection.face) {
    vec3 curLight =
        ambientLight *
        glm::max(0.2f, dot(-intersection.ray,
                           normalize(intersection.face->planeNormal)));
    if (intersection.face->material) {
      color.r += intersection.face->material->diffuse[0] * curLight.r;
      color.g += intersection.face->material->diffuse[1] * curLight.g;
      color.b += intersection.face->material->diffuse[2] * curLight.b;
    }
  }
  return color;
}

vec3 RayCastShader::getColorInternal(const Intersection &intersection, int i) {
  vec3 color{0};
  int samples = i == 0 ? 128 : 1;

  if (i < 4 && intersection.face && intersection.face->material) {
    // TODO: We need to get the roughness of the material, and randomize the ray
    // direction
    float roughness = 0.5;
    auto mtl = intersection.face->material;

    vec3 reflectedColor{0};
    for (int j = 0; j < samples; j++) {
      vec3 dir = normalize(normalize(intersection.face->planeNormal) +
                           normalize(glm::ballRand<float>(1.0)));
      reflectedColor +=
          getColorInternal(scene.castRay(*intersection.pos, dir), i + 1);
    }

    reflectedColor /= samples;

    color = reflectedColor * mtl->diffuse + mtl->emission;
  }

  return color;
}

vec3 WhittedShader::directLighting(const Intersection &isect,
                                   const Material &material) {
  vec3 color{0, 0, 0};

  for (auto &light : scene.lights) {
    switch (light->lightType()) {
    case LightType::AMBIENT:
      color += material.ambient * dynamic_cast<AmbientLight *>(light)->color;
      break;
    case LightType::POINT:
      if (material.diffuse != vec3(0, 0, 0)) {
        auto &pointLight = dynamic_cast<const PointLight &>(*light);
        vec3 lDir = glm::normalize(pointLight.position - *isect.pos);
        const float lDistance = glm::distance(pointLight.position, *isect.pos);

        vec3 normal = normalize(isect.face->planeNormal);
        // TODO should be shading normal, but don't know where to get that...
        float cosL = dot(lDir, normal);

        if (cosL > 0) {
          // Light is NOT behind the primitive
          vec3 shadowRayOrigin = *isect.pos;

          if (dot(lDir, normal) < 0) {
            shadowRayOrigin -= 0.0001f * normal;
          } else {
            shadowRayOrigin += 0.0001f * normal;
          }

          if (scene.visibility(shadowRayOrigin, lDir, lDistance - 0.0001f))
            color += material.diffuse * pointLight.color * cosL;
        }
      }
      break;
    default:
      break;
    }
  }

  return color;
}

vec3 WhittedShader::specularReflection(const Intersection &isect,
                                       const Material &material) {
  float cos = dot(normalize(isect.face->planeNormal), isect.ray);
  vec3 rdir = 2 * cos * isect.face->planeNormal - isect.ray;

  // TODO: ray
  return {0, 0, 0};
}

vec3 WhittedShader::getColor(const Intersection &intersection) {
  vec3 color{0, 0, 0};

  if (!intersection.pos.has_value()) {
    // We hit the background
    // TODO: Return background color!
    return color;
  }

  auto m = intersection.face->material;

  color += directLighting(intersection, *m);
  color += specularReflection(intersection, *m);

  return color;
}
