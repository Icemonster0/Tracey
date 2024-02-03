#include "sampler.hpp"

namespace trc {

Sampler::Sampler() {}

Sampler::Sampler(glm::ivec2 frame_size) {
    fbuf.fill(frame_size, glm::vec3 {0.25f});
}

void Sampler::render(glm::ivec2 frame_size) {
    glm::vec3 color {
        float(frame_size.x) / 800.f,
        float(frame_size.y) / 800.f,
        1.0f,
    };

    if (frame_size == fbuf.get_size())
        fbuf.fill(color);
    else
        fbuf.fill(frame_size, color);
}

Buffer<glm::vec3> *Sampler::get_frame_buffer() {
    return &fbuf;
}

} /* trc */
