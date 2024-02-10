#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include <optional>
#include <cstdint>

#include "../../lib/glm.hpp"
#include "../graphics/buffer.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/shader_pack.hpp"
#include "../geometry/camera.hpp"
#include "../geometry/ray.hpp"
#include "../geometry/shapes/shapes.hpp"
#include "../geometry/intersection.hpp"
#include "accelerator.hpp"

namespace trc {

class Sampler {
public:
    Sampler();
    Sampler(glm::ivec2 frame_size);

    void render(glm::ivec2 frame_size, Camera *camera, Accelerator *accelerator, ShaderPack *shader_pack, uint64_t seed, bool reset, int max_samples);

    Buffer<glm::vec3> *get_frame_buffer();
    int get_samples();

private:
    Buffer<glm::vec3> fbuf;
    int samples;

    void clear_fbuf(glm::ivec2 frame_size, glm::vec3 color);
};

} /* trc */

#endif /* end of include guard: SAMPLER_HPP */
