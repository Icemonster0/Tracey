#include "camera.hpp"

#include "../../glm.hpp"

namespace trc {

Camera::Camera(glm::vec3 p_pos, flaot p_yaw, flaot p_pitch, float p_fov, float p_aspect)
    : pos(p_pos), yaw(p_yaw), pitch(p_pitch), fov(p_fov), aspect(p_aspect) {}

Camera::Camera()
    : pos(0.f), yaw(0.f), pitch(0.f), fov(90.f), aspect(1.f) {}

glm::vec3 Camera::calc_forward_dir() {
    glm::vec3 dir {0.f, 0.f, -1.f};
    dir = glm::rotate(dir, glm::vec3(1.f, 0.f, 0.f), glm::radians(pitch));
    dir = glm::rotate(dir, glm::vec3(0.f, 1.f, 0.f), glm::radians(yaw));
    return dir;
}

glm::vec3 Camera::calc_ray_dir_at(glm::vec2 tex_coord) {
    float abs_height = tan(glm::radians(fov) * 0.5f);
    glm::vec3 dir {
        tex_coord.x * abs_height * aspect,
        tex_coord.y * abs_height,
        -1.f
    };

    dir = glm::normalize(dir);
    dir = glm::rotate(dir, glm::vec3(1.f, 0.f, 0.f), glm::radians(pitch));
    dir = glm::rotate(dir, glm::vec3(0.f, 1.f, 0.f), glm::radians(yaw));
    return dir;
}

} /* trc */
