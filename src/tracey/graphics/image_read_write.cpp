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
    int result = stbi_write_png(path.c_str(), size.x, size.y, 3, data, size.x * 3);
    free(data);
    return result;
}

int read_texture(const char *path, AttribTexture<glm::vec3> &texture) {
    glm::ivec2 size;
    int channels;
    const int force_channels = 3;
    uint8_t *data = stbi_load(path, &size.x, &size.y, &channels, force_channels);

    if (data == nullptr) return 1;

    texture.set_size(size);
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            int offset = y * size.x + x;

            // transform to range [0; 1]
            float r = float(*(data + offset * 3 + 0)) / 255.f;
            float g = float(*(data + offset * 3 + 1)) / 255.f;
            float b = float(*(data + offset * 3 + 2)) / 255.f;

            texture.set_pixel({x, size.y-y-1}, glm::saturate(glm::vec3 {r, g, b}));
        }
    }

    free(data);
    return 0;
}

int read_texture(const char *path, AttribTexture<float> &texture) {
    glm::ivec2 size;
    int channels;
    const int force_channels = 1;
    uint8_t *data = stbi_load(path, &size.x, &size.y, &channels, force_channels);

    if (data == nullptr) return 1;

    texture.set_size(size);
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            int offset = y * size.x + x;

            // transform to range [0; 1]
            float v = float(*(data + offset)) / 255.f;

            texture.set_pixel({x, size.y-y-1}, std::clamp(v, 0.f, 1.f));
        }
    }

    free(data);
    return 0;
}

int read_texture_from_memory(const uint8_t *buffer, int length, AttribTexture<glm::vec3> &texture) {
    glm::ivec2 size;
    int channels;
    const int force_channels = 3;
    uint8_t *data = stbi_load_from_memory(buffer, length, &size.x, &size.y, &channels, force_channels);

    if (data == nullptr) return 1;

    texture.set_size(size);
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            int offset = y * size.x + x;

            // transform to range [0; 1]
            float r = float(*(data + offset * 3 + 0)) / 255.f;
            float g = float(*(data + offset * 3 + 1)) / 255.f;
            float b = float(*(data + offset * 3 + 2)) / 255.f;

            texture.set_pixel({x, size.y-y-1}, glm::saturate(glm::vec3 {r, g, b}));
        }
    }

    free(data);
    return 0;
}

int read_texture_from_memory(const uint8_t *buffer, int length, AttribTexture<float> &texture) {
    glm::ivec2 size;
    int channels;
    const int force_channels = 1;
    uint8_t *data = stbi_load_from_memory(buffer, length, &size.x, &size.y, &channels, force_channels);

    if (data == nullptr) return 1;

    texture.set_size(size);
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            int offset = y * size.x + x;

            // transform to range [0; 1]
            float v = float(*(data + offset)) / 255.f;

            texture.set_pixel({x, size.y-y-1}, std::clamp(v, 0.f, 1.f));
        }
    }

    free(data);
    return 0;
}

} /* trc::image_rw */
