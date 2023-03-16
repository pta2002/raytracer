#pragma once
#include <cstdint>

#include <cmath>
#include <glm/vec3.hpp>

using namespace glm;

class Camera {
private:
  uint32_t width;
  uint32_t height;
  double angle_w;
  double angle_h;
  vec3 up;
  vec3 pos;
  vec3 lookingAt;

public:
  Camera(uint32_t width, uint32_t height, double angle_x, double angle_y,
         vec3 up, vec3 pos, vec3 lookingAt)
      : width{width}, height{height}, angle_w{tan(angle_x / 2)},
        angle_h{tan(angle_y / 2)}, up{up}, pos{pos}, lookingAt{lookingAt} {};

  glm::vec3 getRay(uint32_t x, uint32_t y);

  void render();
};
