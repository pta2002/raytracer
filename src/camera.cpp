#include "camera.hpp"
#include "fmt/core.h"

glm::vec3 Camera::getRay(uint32_t x, uint32_t y) {
  // TODO: Calculate ray
  return {0, 0, 0};
}

void Camera::render() {
  for (uint32_t y = 0; y < height; y++) {
    for (uint32_t x = 0; x < width; x++) {
      glm::vec3 ray = getRay(x, y);

      fmt::println("Ray for {},{}: {} {} {}", x, y, ray.x, ray.y, ray.z);
    }
  }
}