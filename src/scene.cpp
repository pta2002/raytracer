#include "scene.hpp"

#include <optional>
#include <tiny_obj_loader.h>
#include <iostream>

using namespace tinyobj;

static void info(const ObjReader obj) {

  Scene::attributes = obj.GetAttrib();
  Scene::shapes = obj.GetShapes();
  Scene::materials = obj.GetMaterials();

  std::cout << "# of vertices: " << (Scene::attributes.vertices.size() / 3) << std::endl;
  std::cout << "# of normals: " << (Scene::attributes.normals.size() / 3) << std::endl;
  std::cout << "# of textCoords: " << (Scene::attributes.texcoords.size() / 2) << std::endl;
  std::cout << "# of shapes: " << Scene::shapes.size() << std::endl;
  std::cout << "# of materials: " << Scene::materials.size() << std::endl;

  auto it_shapes = Scene::shapes.begin();
  for(; it_shapes != Scene::shapes.end(); it_shapes++) {
    std::cout << "Processing shape " << it_shapes->name << std::endl;
    auto it_vertex = it_shapes->mesh.indices.begin();
    for(; it_vertex != it_shapes->mesh.indices.end(); ) {
      for(int vert=0; vert<3; vert++) {
        std::cout << it_vertex->vertex_index;
        it_vertex++;
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
    printf("There are %lu material indexes.\n", it_shapes->mesh.material_ids.size());
  }
  std::cout << "Shape " << it_shapes->name << " has been processed." << std::endl;
}

// https://github.com/canmom/rasteriser/blob/master/fileloader.cpp
std::optional<Scene> Scene::load(const std::string &filename, const std::string mtlDir) {
  
  ObjReader myObjReader;
  std::string name = "";

  if (!myObjReader.ParseFromFile(filename)) {
    return std::nullopt;
  } else {
    return std::make_optional<Scene>();
  }

  Scene::attributes = myObjReader.GetAttrib();
  Scene::shapes = myObjReader.GetShapes();
  Scene::materials = myObjReader.GetMaterials();
  std::string err;

  auto it_shapes = Scene::shapes.begin();

  if(name != it_shapes->name) { // The first time a new obj is loaded it shows you the info
    info(myObjReader);
    name = it_shapes->name;
  }
  bool success = LoadObj(&attributes, &shapes, &materials, &err, filename.c_str(), mtlDir.c_str(), true);
}

Scene::~Scene() {
  // TODO
}
