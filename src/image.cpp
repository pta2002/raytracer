#include <cstdio>
#include <fmt/core.h>
#include <image.hpp>
#include <spng/spng.h>

Image::Image() {
  imageData = {};
  width = 128;
  height = 64;

  for (int i = 0; i < width * height; i++) {
    imageData.push_back(255);
    imageData.push_back(0);
    imageData.push_back(0);
  }
}

bool Image::writePNG(const std::string &filename) {
  // Let's open the file
  FILE *f = std::fopen(filename.c_str(), "w");
  if (!f) {
    fmt::println(stderr, "Failed to create file");
    return false;
  }

  spng_ctx *ctx = spng_ctx_new(SPNG_CTX_ENCODER);
  spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);

  struct spng_ihdr ihdr = {0};
  ihdr.width = width;
  ihdr.height = height;
  ihdr.color_type = SPNG_COLOR_TYPE_TRUECOLOR;
  ihdr.bit_depth = 8;

  spng_set_ihdr(ctx, &ihdr);
  spng_set_image_limits(ctx, width, height);

  int ret;

  if ((ret = spng_encode_image(ctx, imageData.data(), imageData.size(),
                               SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE))) {
    fmt::println(stderr, "spng encoding error: {}", spng_strerror(ret));
    spng_ctx_free(ctx);
    return false;
  }

  size_t png_size;
  void *png_buf = spng_get_png_buffer(ctx, &png_size, &ret);

  if (ret) {
    fmt::println(stderr, "spng encoding error: {}", spng_strerror(ret));
    return false;
  }

  std::fwrite(png_buf, png_size, 1, f);

  spng_ctx_free(ctx);
  std::fclose(f);

  return true;
}
