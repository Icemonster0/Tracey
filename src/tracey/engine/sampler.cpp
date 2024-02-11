#include "sampler.hpp"

#include <omp.h>

#include "../graphics/shader_data.hpp"
#include "../random/random.hpp"

namespace trc {

/* public */

Sampler::Sampler() {}

Sampler::Sampler(glm::ivec2 frame_size) : samples(0) {
    fbuf.fill(frame_size, glm::vec3 {0.25f});
}

void Sampler::render_frame(glm::ivec2 frame_size, Camera *camera, Accelerator *accelerator, ShaderPack *shader_pack, uint64_t seed, bool reset, int max_samples, bool preview_mode) {
    if (reset) {
        samples = 0;
        clear_fbuf(frame_size, glm::vec3 {0.f});
    }
    if (samples >= max_samples) return;
    ++samples;

    Buffer<glm::vec3> new_sample = preview_mode ? render_preview_sample(frame_size, camera, accelerator, shader_pack, seed)
                                                : render_sample(frame_size, camera, accelerator, shader_pack, seed);

    float inv_samples = 1.f / (float)samples;
    float old_pixel_fac = float(samples - 1) * inv_samples;

    #pragma omp parallel for schedule(static) collapse(2)
    for (int x = 0; x < frame_size.x; ++x) {
        for (int y = 0; y < frame_size.y; ++y) {

            glm::vec3 *old_pixel = fbuf.at({x, y});
            glm::vec3 *new_pixel = new_sample.at({x, y});

            *old_pixel *= old_pixel_fac;
            *old_pixel += *new_pixel * inv_samples;
        }
    }
}

void Sampler::render_image_sample(Camera *camera, Accelerator *accelerator, ShaderPack *shader_pack, uint64_t seed, int sample) {
    Buffer<glm::vec3> new_sample = render_sample(image.get_size(), camera, accelerator, shader_pack, seed);

    float inv_sample = 1.f / (float)sample;
    float old_pixel_fac = float(sample - 1) * inv_sample;

    #pragma omp parallel for schedule(static) collapse(2)
    for (int x = 0; x < image.get_size().x; ++x) {
        for (int y = 0; y < image.get_size().y; ++y) {

            glm::vec3 *old_pixel = image.at({x, y});
            glm::vec3 *new_pixel = new_sample.at({x, y});

            *old_pixel *= old_pixel_fac;
            *old_pixel += *new_pixel * inv_sample;
        }
    }
}

void Sampler::initialize_image(glm::ivec2 size) {
    image.fill(size, glm::vec3 {0.f});
}

void Sampler::destroy_image() {
    image.fill(glm::ivec2 {0}, glm::vec3 {0.f});
}

Buffer<glm::vec3> *Sampler::get_frame_buffer() {
    return &fbuf;
}

Buffer<glm::vec3> *Sampler::get_image() {
    return &image;
}

int Sampler::get_samples() {
    return samples;
}

/* private */

Buffer<glm::vec3> Sampler::render_sample(glm::ivec2 frame_size, Camera *camera, Accelerator *accelerator, ShaderPack *shader_pack, uint64_t seed) {
    Buffer<glm::vec3> buf {frame_size, glm::vec3 {0.f}};

    #pragma omp parallel
    {
        // seed the rng
        int thread_n = omp_get_thread_num();
        RNG rng {seed, seed << thread_n, (seed >> 12) + 0xabc, seed * thread_n >> 31};
        rng.jump(); // offset into the rng, because the first bits are basically independent of the seed

        std::uniform_real_distribution<float> sample_offset_distrib {-0.5f, 0.5f};

        #pragma omp for schedule(static) collapse(2)
        for (int x = 0; x < frame_size.x; ++x) {
            for (int y = 0; y < frame_size.y; ++y) {

                glm::vec2 sample_offset {
                    sample_offset_distrib(rng),
                    sample_offset_distrib(rng)
                };

                glm::vec2 screen_coords {
                    ((float)x + sample_offset.x) * 2.f / (float)frame_size.x - 1.f,
                    ((float)y + sample_offset.y) * 2.f / (float)frame_size.y - 1.f
                };

                Ray ray {
                    camera->get_pos(),
                    camera->calc_ray_dir_at(screen_coords),
                    TRC_CAMERA_RAY,
                    0
                };

                glm::vec3 color {0.f};

                std::optional<Intersection> isect = accelerator->calc_intersection(ray);
                if (isect) {
                    ShaderData shader_data {
                        isect.value().pos,
                        isect.value().normal,
                        isect.value().tex_coord,
                        isect.value().distance,
                        ray,
                        accelerator,
                        shader_pack,
                        &rng
                    };
                    color = isect.value().shader->evaluate(shader_data).rgb();
                }

                *buf.at({x, y}) = color;
            }
        }
    }

    return buf;
}

Buffer<glm::vec3> Sampler::render_preview_sample(glm::ivec2 frame_size, Camera *camera, Accelerator *accelerator, ShaderPack *shader_pack, uint64_t seed) {
    Buffer<glm::vec3> buf {frame_size, glm::vec3 {0.f}};

    #pragma omp parallel for schedule(static) collapse(2)
    for (int x = 0; x < frame_size.x; ++x) {
        for (int y = 0; y < frame_size.y; ++y) {

            glm::vec2 screen_coords {
                (float)x * 2.f / (float)frame_size.x - 1.f,
                (float)y * 2.f / (float)frame_size.y - 1.f
            };

            Ray ray {
                camera->get_pos(),
                camera->calc_ray_dir_at(screen_coords),
                TRC_CAMERA_RAY,
                0
            };

            glm::vec3 color {0.f};

            std::optional<Intersection> isect = accelerator->calc_intersection(ray);
            if (isect) {
                ShaderData shader_data {
                    isect.value().pos,
                    isect.value().normal,
                    isect.value().tex_coord,
                    isect.value().distance,
                    ray,
                    accelerator,
                    shader_pack,
                    nullptr
                };
                color = shader_pack->shader_preview->evaluate(shader_data).rgb();
            }

            *buf.at({x, y}) = color;
        }
    }

    return buf;
}

void Sampler::clear_fbuf(glm::ivec2 frame_size, glm::vec3 color) {
    if (frame_size == fbuf.get_size())
        fbuf.fill(color);
    else
        fbuf.fill(frame_size, color);
}

} /* trc */
