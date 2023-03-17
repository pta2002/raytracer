#include "camera.hpp"
#include "image.hpp"
#include "scene.hpp"
#include <fmt/core.h>

#include <cmath>

int main(int argc, char **argv) {
  // Check command-line arguments
  if (argc != 2) {
    fmt::println(stderr, "USAGE: {} [filename]", argv[0]);
    return -1;
  }

  auto scene = Scene::load(argv[1]);

  if (!scene.has_value()) {
    fmt::println(stderr, "Failed to load scene {}", argv[1]);
    return -1;
  }

  // TODO: O vetor up não está perpendicular com o looking at acho eu?
  Camera cam = Camera(100, 50, M_PI / 2, M_PI / 4, {0, 1, 0}, {0, -1000, -400},
                      {0, 0, 0});

  scene.value().setCamera(cam);
  Image i = scene.value().render();

  i.writePNG("image.png");
}
