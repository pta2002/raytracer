#include "scene.hpp"
#include "geometry.hpp"

#include <fmt/core.h>
#include <iostream>
#include <optional>

#include <tiny_obj_loader.h>

using namespace tinyobj;
using namespace glm;

using std::vector;

void Scene::info() {
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

void Scene::componentToVec2(const vector<float> &components, vector<vec2> &vecs) {
  for (size_t v_start = 0; v_start < components.size(); v_start += 2)
    vecs.push_back(vec2(components[v_start], components[v_start + 1]));
}

void Scene::componentToVec3(const vector<float> components, vector<vec3> &vecs) {
  for (size_t v_start = 0; v_start < components.size(); v_start += 3)
    vecs.push_back(vec3(components[v_start], components[v_start + 1],
                        components[v_start + 2]));
}

void Scene::loadFaces(shape_t & shape, vector<Triangle> & faces) {
  const vector<index_t> & indices = shape.mesh.indices;
  const vector<int> & mat_idx = shape.mesh.material_ids;
  
  fmt::print("# Loading {} triangles...", mat_idx.size());

  for(size_t face_idx=0; face_idx < mat_idx.size(); face_idx++) {
    faces.push_back(
      Triangle(
        {indices[3*face_idx].vertex_index, indices[3*face_idx+1].vertex_index, indices[3*face_idx+2].vertex_index},
        {indices[3*face_idx].normal_index, indices[3*face_idx+1].normal_index, indices[3*face_idx+2].normal_index},
        {indices[3*face_idx].texcoord_index, indices[3*face_idx+1].texcoord_index, indices[3*face_idx+2].texcoord_index},
        mat_idx[face_idx]
      )
    );
  }

}

// https://github.com/canmom/rasteriser/blob/master/fileloader.cpp

// std::optional<Scene> Scene::load(const std::string &filename, const
// std::string mats)
std::optional<Scene> Scene::load(const std::string &filename, 
                                 const std::string &mats,
                                 vector<Triangle> &triangles,
                                 vector<vec3> &vertices,
                                 vector<vec3> &vertNormal, 
                                 vector<vec2> &uvs) {
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

  if (!err.empty()) std::cerr << err << std::endl;

  scene.componentToVec3(scene.attributes.vertices, vertices);

  scene.componentToVec3(scene.attributes.normals, vertNormal);

  scene.componentToVec2(scene.attributes.texcoords, uvs);

  for(auto shape : scene.shapes) scene.loadFaces(shape, triangles);

  return {scene};
}

Scene::~Scene() = default;
