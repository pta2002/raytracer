#include "scene.hpp"

#include <fmt/core.h>
#include <optional>
#include <tiny_obj_loader.h>

using namespace tinyobj;

void Scene::info(void) {
  fmt::println("# of vertices: {}", attributes.vertices.size() / 3);
  fmt::println("# of normals: {}", attributes.normals.size() / 3);
  fmt::println("# of textCoords: {}", attributes.texcoords.size() / 2);
  fmt::println("# of shapes: {}", shapes.size());
  fmt::println("# of materials: {}", materials.size());

  for (auto shape : shapes) {
    fmt::println("Processing shape {}", shape.name);
    for (auto vertex : shape.mesh.indices) {
      for (int vert = 0; vert < 3; vert++) {
        fmt::print("{} ", vertex.vertex_index);
      }
      fmt::println("");
    }
    fmt::println("There are {} material indices.",
                 shape.mesh.material_ids.size());
    fmt::println("Shape {} has been processed.", shape.name);
  }
}

// https://github.com/canmom/rasteriser/blob/master/fileloader.cpp
std::optional<Scene> Scene::load(const std::string &filename) {
  ObjReader myObjReader;

  if (!myObjReader.ParseFromFile(filename)) {
    return std::nullopt;
  }

  Scene scene;

  scene.attributes = myObjReader.GetAttrib();
  scene.shapes = myObjReader.GetShapes();
  scene.materials = myObjReader.GetMaterials();
  std::string err;

  scene.info();

  return {scene};

  //  auto it_shapes = Scene::shapes.begin();
  //
  //  if (name != it_shapes->name) {
  //    // The first time an object is loaded it shows you its info
  //    scene.info(myObjReader);
  //    name = it_shapes->name;
  //  }
}

Scene::~Scene() = default;
