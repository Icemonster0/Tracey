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

    void render_frame(glm::ivec2 frame_size, Camera *camera, Accelerator *accelerator,
        ShaderPack *shader_pack, uint64_t seed, bool reset, int max_samples,
        bool preview_mode, float exposure);
    void render_image_sample(Camera *camera, Accelerator *accelerator,
        ShaderPack *shader_pack, uint64_t seed, int sample, float exposure);

    void initialize_image(glm::ivec2 size);
    void destroy_image();

    Buffer<glm::vec3> *get_frame_buffer();
    Buffer<glm::vec3> *get_image();
    int get_samples();

private:
    Buffer<glm::vec3> fbuf;
    int samples;

    Buffer<glm::vec3> image;

    Buffer<glm::vec3> render_sample(glm::ivec2 frame_size, Camera *camera, Accelerator *accelerator, ShaderPack *shader_pack, uint64_t seed);
    Buffer<glm::vec3> render_preview_sample(glm::ivec2 frame_size, Camera *camera, Accelerator *accelerator, ShaderPack *shader_pack, uint64_t seed);
    void clear_fbuf(glm::ivec2 frame_size, glm::vec3 color);
};

} /* trc */

#endif /* end of include guard: SAMPLER_HPP */
