#ifndef IMAGE_READ_WRITE_HPP
#define IMAGE_READ_WRITE_HPP

#include <string>

#include "../../lib/glm.hpp"
#include "../../lib/stb.hpp"
#include "buffer.hpp"

namespace trc::image_rw {

int write_png(Buffer<glm::vec3> *buf, std::string path);

} /* trc::image_rw */

#endif /* end of include guard: IMAGE_READ_WRITE_HPP */
