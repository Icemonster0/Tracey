#include "image_read_write.hpp"

#include <cstdint>
#include <algorithm>

namespace trc::image_rw {

int write_png(Buffer<glm::vec3> *buf, std::string path) {
    glm::ivec2 size = buf->get_size();

    uint8_t *data = (uint8_t*)malloc(size.x * size.y * 3);
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            int offset = (y * size.x + x) * 3;
            glm::vec3 color = *buf->at({x, y});
            data[offset + 0] = (uint8_t)std::clamp((int)(color.r * 255), 0, 255);
            data[offset + 1] = (uint8_t)std::clamp((int)(color.g * 255), 0, 255);
            data[offset + 2] = (uint8_t)std::clamp((int)(color.b * 255), 0, 255);
        }
    }

    stbi_flip_vertically_on_write(1);
    return stbi_write_png(path.c_str(), size.x, size.y, 3, data, size.x * 3);
}

} /* trc::image_rw */
