#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Image {
private:
  std::vector<uint8_t> imageData;

public:
  uint32_t width;
  uint32_t height;

  Image();
  bool writePNG(const std::string &filename);
};