#pragma once
#include <cstdint>

#include <glm/vec3.hpp>

class Camera {
private:
  uint32_t width;
  uint32_t height;
  uint32_t angle;

public:
  Camera(uint32_t width, uint32_t height, uint32_t angle)
      : width{width}, height{height}, angle{angle} {};

  glm::vec3 getRay(uint32_t x, uint32_t y);

  void render();
};
