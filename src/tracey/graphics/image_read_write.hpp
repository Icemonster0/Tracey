#ifndef IMAGE_READ_WRITE_HPP
#define IMAGE_READ_WRITE_HPP

#include <string>

#include "../../lib/glm.hpp"
#include "../../lib/stb.hpp"
#include "buffer.hpp"
#include "attribute.hpp"

namespace trc::image_rw {

int write_png(Buffer<glm::vec3> *buf, std::string path);

int read_texture(const char *path, AttribTexture<glm::vec3> &texture);
int read_texture(const char *path, AttribTexture<float> &texture);

int read_texture_from_memory(const uint8_t *buffer, int length, AttribTexture<glm::vec3> &texture);
int read_texture_from_memory(const uint8_t *buffer, int length, AttribTexture<float> &texture);

} /* trc::image_rw */

#endif /* end of include guard: IMAGE_READ_WRITE_HPP */
