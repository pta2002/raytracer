#pragma once

#include <vector>
#include <tiny_obj_loader.h>

class Light {
public:
    Light() = default;

    std::vector<float> rgb;

    /**
     * Returns the color of the point as affected by this light source
    */
    virtual std::vector<float> light(tinyobj::attrib_t attributes, tinyobj::material_t material);
};

class AmbientLight : public Light {
public:
    AmbientLight() = default;

    std::vector<float> light(tinyobj::attrib_t attributes, tinyobj::material_t material);
};