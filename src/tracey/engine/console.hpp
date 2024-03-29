#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "../../lib/glm.hpp"

namespace trc {

class Console {
public:
    Console();

    void print(float frequency, float delta_t, bool preview_mode, int samples,
        int max_samples, glm::ivec2 window_size, glm::vec3 view_pos, float yaw,
        float pitch, float fov, float speed, float focal_length, float aperture);
    void print_render_info(float frequency, float delta_t, int sample,
        int max_samples, glm::ivec2 image_size, float time, float sample_rate,
        bool render_only);
    void clear();

private:
    float print_cooldown;
};

} /* trc */

#endif /* end of include guard: CONSOLE_HPP */
