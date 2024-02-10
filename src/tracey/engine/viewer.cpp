#include "viewer.hpp"

#include <algorithm>

namespace trc {

Viewer::Viewer(glm::ivec2 window_size, float fov)
    : look_sens(0.1f), scroll_fac(1.1f), move_speed(5.0f) {
    camera = Camera {
        glm::vec3 {0.f}, // position
        0.f, // pitch
        0.f, // yaw
        fov, // fov
        (float)window_size.x / (float)window_size.y // aspect ratio
    };
    window_size_cache = window_size;
}

Viewer::Viewer() {}

void Viewer::update(InputPackage input, float delta_t, glm::ivec2 window_size) {
    if (window_size != window_size_cache) {
        window_size_cache = window_size;
        camera.set_aspect((float)window_size.x / (float)window_size.y);
    }

    camera.set_yaw(camera.get_yaw() + input.delta_mouse.x * look_sens);
    camera.set_pitch(std::clamp(camera.get_pitch() + input.delta_mouse.y * look_sens, -90.f, 90.f));

    move_speed *= powf(scroll_fac, input.delta_scroll.y);
    move_speed = std::clamp(move_speed, 0.1f, 100.f);

    glm::ivec3 move_input {
        (input.d ? 1 : 0) - (input.a ? 1 : 0),
        (input.e ? 1 : 0) - (input.q ? 1 : 0),
        (input.w ? 1 : 0) - (input.s ? 1 : 0)
    };
    glm::vec3 move_dir = move_input.x * camera.calc_right_dir()
                       + move_input.y * glm::vec3(0.f, 1.f, 0.f)
                       + move_input.z * camera.calc_forward_dir();
    if (move_dir.x || move_dir.y || move_dir.z)
        move_dir = glm::normalize(move_dir);

    camera.set_pos(camera.get_pos() + move_dir * move_speed * delta_t);
}

Camera *Viewer::get_camera() {
    return &camera;
}

float Viewer::get_speed() {
    return move_speed;
}

} /* trc */
