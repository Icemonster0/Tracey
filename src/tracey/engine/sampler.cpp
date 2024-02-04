#include "sampler.hpp"

namespace trc {

/* public */

Sampler::Sampler() {}

Sampler::Sampler(glm::ivec2 frame_size) {
    fbuf.fill(frame_size, glm::vec3 {0.25f});
}

void Sampler::render(glm::ivec2 frame_size, Camera *camera, Accelerator *accelerator) {
    clear_fbuf(frame_size, glm::vec3 {0.f});

    #pragma omp parallel for schedule(static) collapse(2)
    for (int x = 0; x < frame_size.x; ++x) {
        for (int y = 0; y < frame_size.y; ++y) {
            glm::vec2 screen_coords {
                (float)x*2.f / (float)frame_size.x - 1.f,
                (float)y*2.f / (float)frame_size.y - 1.f
            };

            Ray ray {
                camera->get_pos(),
                camera->calc_ray_dir_at(screen_coords)
            };

            std::optional<Intersection> isect = accelerator->calc_intersection(ray);
            if (isect) {
                glm::vec3 color = isect.value().shader->evaluate(
                    isect.value().pos,
                    isect.value().normal,
                    isect.value().tex_coord,
                    isect.value().distance
                );
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
