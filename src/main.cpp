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

  Camera cam =
      Camera(4, 4, M_PI / 2, M_PI / 4, {0, 1, 0}, {0, 0, 0}, {0, 0, 1});
  //  cam.render();

  //  Image i = Image();
  //
  //  i.writePNG("image.png");
}
