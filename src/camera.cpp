#include "camera.hpp"
#include "fmt/core.h"

#include <glm/glm.hpp>
#include <glm/mat3x3.hpp>

glm::vec3 Camera::getRay(uint32_t x, uint32_t y) {
  // TODO: Calculate ray
  double xs = 2 * (x + 0.5) / width - 1;
  double ys = 2 * (y + 0.5) / height - 1;

  double xc = xs * angle_w;
  double yc = ys * angle_h;

  vec3 forward = normalize(lookingAt - pos);
  vec3 right = normalize(cross(up, forward));

  mat3x3 c2w{right, up, forward};

  return normalize(c2w * vec3(xc, yc, 1));
}

void Camera::render() {
  for (uint32_t y = 0; y < height; y++) {
    for (uint32_t x = 0; x < width; x++) {
      glm::vec3 ray = getRay(x, y);

      fmt::println("Ray for {},{}: {} {} {}", x, y, ray.x, ray.y, ray.z);
    }
  }
}