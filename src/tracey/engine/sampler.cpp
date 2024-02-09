#include "sampler.hpp"

#include <omp.h>

#include "../graphics/shader_data.hpp"
#include "../random/random.hpp"

namespace trc {

/* public */

Sampler::Sampler() {}

Sampler::Sampler(glm::ivec2 frame_size) {
    fbuf.fill(frame_size, glm::vec3 {0.25f});
}

void Sampler::render(glm::ivec2 frame_size, Camera *camera, Accelerator *accelerator, ShaderPack *shader_pack, uint64_t seed) {
    clear_fbuf(frame_size, glm::vec3 {0.f});

    #pragma omp parallel for schedule(static) collapse(2)
    for (int x = 0; x < frame_size.x; ++x) {
        for (int y = 0; y < frame_size.y; ++y) {

            // seed the rng with arbitrary numbers that depend on seed, x and y
            RNG rng {(uint64_t)x * y + seed, (uint64_t)y + x - seed, (uint64_t)x << 6 * seed, (uint64_t)y >> 9 * seed};
            rng.jump(); // offset into the rng, because the first bits are basically independent of the seed

            std::uniform_real_distribution<float> sample_offset_distrib {-0.5f, 0.5f};

            glm::vec2 screen_coords {
                ((float)x + sample_offset_distrib(rng)) * 2.f / (float)frame_size.x - 1.f,
                ((float)y + sample_offset_distrib(rng)) * 2.f / (float)frame_size.y - 1.f
            };

            Ray ray {
                camera->get_pos(),
                camera->calc_ray_dir_at(screen_coords),
                TRC_CAMERA_RAY,
                0
            };

            std::optional<Intersection> isect = accelerator->calc_intersection(ray);
            if (isect) {
                ShaderData shader_data {
                    isect.value().pos,
                    isect.value().normal,
                    isect.value().tex_coord,
                    isect.value().distance,
                    ray,
                    accelerator,
                    shader_pack
                };
                glm::vec3 color = isect.value().shader->evaluate(shader_data);
                *fbuf.at({x, y}) = color.rgb();
            }
        }
    }
}

Buffer<glm::vec3> *Sampler::get_frame_buffer() {
    return &fbuf;
}

/* private */

void Sampler::clear_fbuf(glm::ivec2 frame_size, glm::vec3 color) {
    if (frame_size == fbuf.get_size())
        fbuf.fill(color);
    else
        fbuf.fill(frame_size, color);
}

} /* trc */
