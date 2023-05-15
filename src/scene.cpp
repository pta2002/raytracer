#include "scene.hpp"
#include "geometry.hpp"
#include "light.hpp"

#include <fmt/color.h>
#include <fmt/core.h>
#include <fstream>
#include <memory>
#include <optional>

#include "shader.hpp"
#include <nlohmann/json.hpp>
#include <tiny_obj_loader.h>

using namespace tinyobj;
using namespace glm;

SceneDef::SceneDef(const std::string &filename) {
  std::ifstream js(filename);
  auto data = nlohmann::json::parse(js);

  modelFile = data["model"];
  outputFile = data["output"];
  width = data["width"];
  height = data["height"];

  auto &cameraDef = data["frames"][0]["camera"];

  camera = make_shared<Camera>(
      Camera(width, height, cameraDef["fov"]["x"], cameraDef["fov"]["y"],
             {cameraDef["up"]["x"], cameraDef["up"]["y"], cameraDef["up"]["z"]},
             {cameraDef["position"]["x"], cameraDef["position"]["y"],
              cameraDef["position"]["z"]},
             {cameraDef["lookingAt"]["x"], cameraDef["lookingAt"]["y"],
              cameraDef["lookingAt"]["z"]}));

  for (auto &light : data["lights"]) {
    if (light["type"] == "point") {
      auto l = new PointLight(
          {light["pos"]["x"], light["pos"]["y"], light["pos"]["z"]},
          {light["color"]["r"], light["color"]["g"], light["color"]["b"]});

      fmt::print(fmt::emphasis::bold | fg(fmt::color::yellow), "[light] ");
      fmt::println("Loading point light {} {} {}", l->color.x, l->color.y,
                   l->color.z);

      lights.push_back(l);
    } else if (light["type"] == "ambient") {
      auto l = new AmbientLight(
          {light["color"]["r"], light["color"]["g"], light["color"]["b"]});
      lights.push_back(l);
    }
  }
}

optional<unique_ptr<Scene>> SceneDef::getScene() {
  auto ret = Scene::load(modelFile);

  if (ret) {
    ret.value()->setCamera(*camera);
    ret.value()->lights = std::move(lights);
  }

  return std::move(ret);
}

// https://github.com/canmom/rasteriser/blob/master/fileloader.cpp
// std::optional<Scene> Scene::load(const std::string &filename, const
// std::string mats)
std::optional<unique_ptr<Scene>> Scene::load(const std::string &filename) {
  ObjReader myObjReader;

  if (!myObjReader.ParseFromFile(filename)) {
    return std::nullopt;
  }

  auto scene = std::make_unique<Scene>();

  scene->attributes = myObjReader.GetAttrib();
  scene->shapes = myObjReader.GetShapes();
  scene->materials = {};

  // Materials
  for (auto &material : myObjReader.GetMaterials()) {
    scene->materials.emplace_back(material);
  }

  // Objects
  for (auto &shape : scene->shapes) {
    auto &indices = shape.mesh.indices;
    auto &mat_ids = shape.mesh.material_ids;

    fmt::print(fmt::emphasis::bold | fg(fmt::color::yellow), "[{}] ",
               shape.name);
    fmt::println("Loading {} materials, {} vertices...", mat_ids.size(),
                 indices.size());

    Object obj{};

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
          vec3(scene->attributes.vertices[vertexIndices[0] * 3],
               scene->attributes.vertices[vertexIndices[0] * 3 + 1],
               scene->attributes.vertices[vertexIndices[0] * 3 + 2]),
          vec3(scene->attributes.vertices[vertexIndices[1] * 3],
               scene->attributes.vertices[vertexIndices[1] * 3 + 1],
               scene->attributes.vertices[vertexIndices[1] * 3 + 2]),
          vec3(scene->attributes.vertices[vertexIndices[2] * 3],
               scene->attributes.vertices[vertexIndices[2] * 3 + 1],
               scene->attributes.vertices[vertexIndices[2] * 3 + 2])};

      std::optional<array<vec3, 3>> normals = {};

      if (normalIndices[0] >= 0) {
        normals = {vec3(scene->attributes.normals[normalIndices[0] * 3],
                        scene->attributes.normals[normalIndices[0] * 3 + 1],
                        scene->attributes.normals[normalIndices[0] * 3 + 2]),
                   vec3(scene->attributes.normals[normalIndices[1] * 3],
                        scene->attributes.normals[normalIndices[1] * 3 + 1],
                        scene->attributes.normals[normalIndices[1] * 3 + 2]),
                   vec3(scene->attributes.normals[normalIndices[2] * 3],
                        scene->attributes.normals[normalIndices[2] * 3 + 1],
                        scene->attributes.normals[normalIndices[2] * 3 + 2])};
      }

      int materialIndex = mat_ids[face_id];
      const Material *material = nullptr;
      if (materialIndex >= 0)
        material = &scene->materials[materialIndex];

      // TODO: texCoord
      obj.faces.emplace_back(vertices, normals, vec3(0, 0, 0), material);
    }

    obj.calculateBoundingBox();
    obj.name = shape.name;
    scene->objects.push_back(obj);
  }

  return {std::move(scene)};
}

void Scene::setCamera(const Camera &newCamera) { this->camera = &newCamera; }

void printProgress(uint32_t height, uint32_t current) {
  fmt::print(fmt::emphasis::bold | fg(fmt::color::green), "\r[progress] ");
  fmt::print("{}% done", (float)(current + 1) / (float)height * 100);
  fflush(stdout);
}

Image Scene::render() {
  if (this->camera == nullptr) {
    throw std::runtime_error{"Camera must be set"};
  }

  fmt::print(fmt::emphasis::bold | fg(fmt::color::blue), "[info] ");
  fmt::println("Starting render");

  //  AmbientShader shader(*this, {1, 1, 1});
  //  RayCastShader shader(*this);
  WhittedShader shader(*this);

  Image img{camera->width, camera->height};

  fmt::print(fmt::emphasis::bold | fg(fmt::color::blue), "[info] ");
  fmt::println("width: {} height: {}", camera->width, camera->height);

  for (uint32_t y = 0; y < camera->height; y++) {
    for (uint32_t x = 0; x < camera->width; x++) {
      auto ray = camera->getRay(x, y);
      //      auto ray = camera->getRay(1920 / 2, 0);

      vec3 color = shader.getColor(castRay(camera->pos, ray));
      color = sqrt(color);
      color = clamp(color, 0.0f, 0.999f);

      img.imageData[(y * camera->width + x) * 3] =
          static_cast<unsigned char>(color.r * 256);
      img.imageData[(y * camera->width + x) * 3 + 1] =
          static_cast<unsigned char>(color.g * 256);
      img.imageData[(y * camera->width + x) * 3 + 2] =
          static_cast<unsigned char>(color.b * 256);
    }

    printProgress(camera->height, y);
  }

  fmt::print(fmt::emphasis::bold | fg(fmt::color::blue), "\n[info] ");
  fmt::println("Finished rendering");

  return img;
}

Intersection Scene::castRay(vec3 origin, vec3 direction) const {
  float dist = FLT_MAX;
  optional<vec3> intersection;
  optional<vec3> finalIntersection;
  const Triangle *intersectedFace = nullptr;
  const Object *intersectedObj = nullptr;

  vec3 rayInverse{1 / direction.x, 1 / direction.y, 1 / direction.z};
  for (auto &object : objects) {
    if (object.intersects(direction, origin, rayInverse)) {
      for (auto &face : object.faces) {
        // TODO: We need to filter out objects that are BEHIND the origin, but
        // the question here is... how?
        // 1. p = the point of the object closest, note- this may not be a
        // vertex...
        // 2. objDir = origin - p
        // 3. dot(direction, objDir) > 0 means that it's in front
        if ((intersection = face.intersects(direction, origin)).has_value()) {
          finalIntersection = intersection;
          float new_dist = distance(origin, *intersection);
          if (dist > new_dist) {
            dist = new_dist;
            intersectedFace = &face;
            intersectedObj = &object;
          }
        }
      }
    }
  }

  if (intersectedObj && intersectedObj->name == "Cube.001") {
    fmt::println("Came from {},{},{}, ray {},{},{}", origin.x, origin.y,
                 origin.z, direction.x, direction.y, direction.z);
  }

  return {direction, finalIntersection, intersectedFace};
}

bool Scene::visibility(vec3 origin, vec3 direction, const float maxL) const {
  vec3 rayInverse{1 / direction.x, 1 / direction.y, 1 / direction.z};

  for (auto &object : objects) {
    if (object.intersects(direction, origin, rayInverse)) {
      for (auto &face : object.faces) {
        auto isect = face.intersects(direction, origin);
        if (isect) {
          if (distance(origin, *isect) < maxL) {
            return false;
          }
        }
      }
    }
  }

  return true;
}

Scene::~Scene() {
  // Dealocate every light
  for (auto &light : lights) {
    delete light;
  }
}
