#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "../../glm.hpp"
#include "../graphics/buffer.hpp"

namespace trc {

class Sampler {
public:
    Sampler();
    Sampler(glm::ivec2 frame_size);

    void render(glm::ivec2 frame_size);

    Buffer<glm::vec3> *get_frame_buffer();

private:
    Buffer<glm::vec3> fbuf;
};

} /* trc */

#endif /* end of include guard: SAMPLER_HPP */
