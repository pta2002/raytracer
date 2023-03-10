#include <fmt/core.h>
#include "scene.hpp"

int main(int argc, char **argv) {
  if (argc == 3) {
    auto scene = Scene::load(argv[1], argv[2]);
    if (scene.has_value()) {
      fmt::println("Loaded scene");
    } else {
      fmt::println(stderr, "Failed to load scene {}", argv[1]);
      return -1;
    }
  } else {
    fmt::println(stderr, "USAGE: {} [filename] [materials directory]", argv[0]);
    return -1;
  }
}
