#include "scene.hpp"

#include <fmt/core.h>
#include <iostream>
#include <optional>
#include <tiny_obj_loader.h>

using namespace tinyobj;

void Scene::info(const ObjReader &obj) {
  attributes = obj.GetAttrib();
  shapes = obj.GetShapes();
  materials = obj.GetMaterials();

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
std::optional<Scene> Scene::load(const std::string &filename,
                                 const std::string &mtlDir) {

  ObjReader myObjReader;
  std::string name = "";

  if (!myObjReader.ParseFromFile(filename)) {
    return std::nullopt;
  } else {
    return std::make_optional<Scene>();
  }

  /* TODO
  Scene::attributes = myObjReader.GetAttrib();
  Scene::shapes = myObjReader.GetShapes();
  Scene::materials = myObjReader.GetMaterials();
  std::string err;

  auto it_shapes = Scene::shapes.begin();

  if(name != it_shapes->name) { // The first time a new obj is loaded it shows
  you the info info(myObjReader); name = it_shapes->name;
  }
  bool success = LoadObj(&attributes, &shapes, &materials, &err,
  filename.c_str(), mtlDir.c_str(), true);
   */
}

Scene::~Scene() {
  // TODO
}
