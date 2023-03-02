#include "scene.hpp"

#include <optional>
#include <tiny_obj_loader.h>

std::optional<Scene> Scene::load(const std::string &filename) {
  tinyobj::ObjReader myObjReader;

  if (!myObjReader.ParseFromFile(filename)) {
    return std::nullopt;
  } else {
    return std::make_optional<Scene>();
  }
}

Scene::~Scene() {
  // TODO
}
