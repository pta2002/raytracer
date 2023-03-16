#include "scene.hpp"

#include <fmt/core.h>
#include <optional>
#include <iostream>

#include <tiny_obj_loader.h>
#include <vector2.hpp>
#include <vector3.hpp>

using namespace tinyobj;

using std::vector;

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

void componentToVec2(const vector<float> components, vector<vec2>& vecs) {
    for(size_t v_start=0; v_start < components.size(); v_start+=2) 
      vecs.push_back(vec2(components[v_start], components[v_start+1]));
}

void componentToVec3(const vector<float> components, vector<vec3>& vecs) {
    for(size_t v_start=0; v_start < components.size(); v_start+=3)
      vecs.push_back(vec3(components[v_start], components[v_start+1], components[v_start+2]));
}

// https://github.com/canmom/rasteriser/blob/master/fileloader.cpp

// std::optional<Scene> Scene::load(const std::string &filename, const std::string mats)
std::optional<Scene> Scene::load(const std::string &filename, vector<vec3> &vertices, vector<vec3> & vertNormal, vector<vec2>& uvs) {
  ObjReader myObjReader;

  if (!myObjReader.ParseFromFile(filename)) {
    return std::nullopt;
  }

  Scene scene;

  scene.attributes = myObjReader.GetAttrib();
  scene.shapes = myObjReader.GetShapes();
  scene.materials = myObjReader.GetMaterials();
  std::string err;

  //  auto it_shapes = Scene::shapes.begin();
  //
  //  if (name != it_shapes->name) {
  //    // The first time an object is loaded it shows you its info
  //    scene.info(myObjReader);
  //    name = it_shapes->name;
  //  }

  scene.info();

  /** bool success = tinyobj::LoadObj(&attributes,
                                  &shapes, 
                                  &materials, 
                                  &err,
                                  filename,
                                  mats,
                                  true); */
  
  if(!err.empty()) std::cerr << err << std::endl;

  // if(!success) exit(1)

  componentToVec3(scene.attributes.vertices, vertices);
  
  componentToVec3(scene.attributes.normals, vertNormal);

  componentToVec2(scene.attributes.texcoords, uvs);

  return {scene};

}

Scene::~Scene() = default;
