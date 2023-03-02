#include "fmt/core.h"
#include "scene.hpp"
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc == 2) {
    auto scene = Scene::load(argv[1]);
    if (scene.has_value()) {
      fmt::println("Loaded scene");
    } else {
      fmt::println(stderr, "Failed to load scene");
      return -1;
    }
  } else {
    fmt::println(stderr, "USAGE: {} [filename]", argv[0]);
    return -1;
  }
}
