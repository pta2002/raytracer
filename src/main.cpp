#include "image.hpp"
#include "scene.hpp"
#include <fmt/core.h>

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

  Image i = Image();

  i.writePNG("image.png");
}
