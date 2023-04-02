#include "scene.hpp"
#include "geometry.hpp"
#include "light.hpp"

#include <fmt/color.h>
#include <fmt/core.h>
#include <fstream>
#include <optional>

#include "shader.hpp"
#include <nlohmann/json.hpp>
#include <tiny_obj_loader.h>

using namespace tinyobj;
using namespace glm;
using std::vector;

class SceneDef {
public:
  std::string modelFile;

  explicit SceneDef(const std::string &filename) {
    std::ifstream js(filename);
    auto data = nlohmann::json::parse(js);

    modelFile = data["model"];
  }
};

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

// https://github.com/canmom/rasteriser/blob/master/fileloader.cpp
// std::optional<Scene> Scene::load(const std::string &filename, const
// std::string mats)
std::optional<Scene> Scene::load(const std::string &filename) {
  SceneDef def = SceneDef(filename);

  ObjReader myObjReader;

  if (!myObjReader.ParseFromFile(def.modelFile)) {
    return std::nullopt;
  }

  Scene scene;

  scene.attributes = myObjReader.GetAttrib();
  scene.shapes = myObjReader.GetShapes();
  scene.materials = myObjReader.GetMaterials();

  for (auto &shape : scene.shapes) {
    auto &indices = shape.mesh.indices;
    auto &mat_ids = shape.mesh.material_ids;

    fmt::print(fmt::emphasis::bold | fg(fmt::color::yellow), "[{}] ",
               shape.name);
    fmt::println("Loading {} materials, {} vertices...", mat_ids.size(),
                 indices.size());

    for (auto face_id = 0; face_id < shape.mesh.material_ids.size();
         face_id++) {
      array<int, 3> vertexIndices{indices[3 * face_id].vertex_index,
                                  indices[3 * face_id + 1].vertex_index,
                                  indices[3 * face_id + 2].vertex_index};

      array<int, 3> normalIndices{indices[3 * face_id].normal_index,
                                  indices[3 * face_id + 1].normal_index,
                                  indices[3 * face_id + 2].normal_index};

      array<int, 3> texcoordIndices{indices[3 * face_id].texcoord_index,
                                    indices[3 * face_id + 1].texcoord_index,
                                    indices[3 * face_id + 2].texcoord_index};

      array<vec3, 3> vertices{
          vec3(scene.attributes.vertices[vertexIndices[0] * 3],
               scene.attributes.vertices[vertexIndices[0] * 3 + 1],
               scene.attributes.vertices[vertexIndices[0] * 3 + 2]),
          vec3(scene.attributes.vertices[vertexIndices[1] * 3],
               scene.attributes.vertices[vertexIndices[1] * 3 + 1],
               scene.attributes.vertices[vertexIndices[1] * 3 + 2]),
          vec3(scene.attributes.vertices[vertexIndices[2] * 3],
               scene.attributes.vertices[vertexIndices[2] * 3 + 1],
               scene.attributes.vertices[vertexIndices[2] * 3 + 2])};

      std::optional<array<vec3, 3>> normals = {};

      if (normalIndices[0] > 0) {
        normals = {vec3(scene.attributes.normals[normalIndices[0] * 3],
                        scene.attributes.normals[normalIndices[0] * 3 + 1],
                        scene.attributes.normals[normalIndices[0] * 3 + 2]),
                   vec3(scene.attributes.normals[normalIndices[1] * 3],
                        scene.attributes.normals[normalIndices[1] * 3 + 1],
                        scene.attributes.normals[normalIndices[1] * 3 + 2]),
                   vec3(scene.attributes.normals[normalIndices[2] * 3],
                        scene.attributes.normals[normalIndices[2] * 3 + 1],
                        scene.attributes.normals[normalIndices[2] * 3 + 2])};
      }

      int materialIndex = mat_ids[face_id];
      const tinyobj::material_t *material = &scene.materials[materialIndex];
      // TODO: texCoord
      scene.faces.emplace_back(vertices, normals, vec3(0, 0, 0), material);
    }
  }

  return {scene};
}
void Scene::setCamera(const Camera &newCamera) { this->camera = &newCamera; }

Scene::~Scene() = default;

Image Scene::render() {
  if (this->camera == nullptr) {
    throw std::runtime_error{"Camera must be set"};
  }

  fmt::print(fmt::emphasis::bold | fg(fmt::color::blue), "[info] ");
  fmt::println("Starting render");

  AmbientShader shader(*this, {1, 1, 1});

  Image img{camera->width, camera->height};

  fmt::print(fmt::emphasis::bold | fg(fmt::color::blue), "[info] ");
  fmt::println("width: {} height: {}", camera->width, camera->height);
  int pixel = 0;
  for (uint32_t y = 0; y < camera->height; y++) {
    for (uint32_t x = 0; x < camera->width; x++) {
      auto ray = camera->getRay(x, y);

      const tinyobj::material_t *mat = nullptr;
      float dist = FLT_MAX;
      optional<vec3> intersection;
      Triangle *intersectedFace = nullptr;
      for (auto &face : faces) {
        if ((intersection = face.intersects(ray, this->camera->pos))) {
          float new_dist = distance(this->camera->pos, *intersection);
          if (dist > new_dist) {
            dist = new_dist;
            intersectedFace = &face;
          }
        }
      }

      vec3 color = shader.getColor(ray, intersection, intersectedFace);

      // TODO: proper gamma correction
      img.imageData.insert(img.imageData.end(),
                           {static_cast<unsigned char>(color.r * 255),
                            static_cast<unsigned char>(color.g * 255),
                            static_cast<unsigned char>(color.b * 255)});
    }
  }

  fmt::print(fmt::emphasis::bold | fg(fmt::color::blue), "[info] ");
  fmt::println("Finished rendering");

  return img;
}