#include "camera.hpp"

#include "../util/math_util.hpp"

namespace trc {

/* public */

Camera::Camera(glm::vec3 p_pos, float p_yaw, float p_pitch, float p_fov, float p_aspect, float p_focal_length, float p_aperture)
    : pos(p_pos), yaw(p_yaw), pitch(p_pitch), fov(p_fov), aspect(p_aspect), focal_length(p_focal_length), aperture(p_aperture) {
    calc_rot_mat();
    calc_abs_height();
}

Camera::Camera()
    : pos(0.f), yaw(0.f), pitch(0.f), fov(90.f), aspect(1.f), focal_length(10.f), aperture(0.f) {
    calc_rot_mat();
    calc_abs_height();
}

void Camera::set_pos(glm::vec3 p_pos) {pos = p_pos;}
void Camera::set_yaw(float p_yaw) {yaw = p_yaw; calc_rot_mat();}
void Camera::set_pitch(float p_pitch) {pitch = p_pitch; calc_rot_mat();}
void Camera::set_fov(float p_fov) {fov = p_fov; calc_abs_height();}
void Camera::set_aspect(float p_aspect) {aspect = p_aspect;}
void Camera::set_focal_length(float p_focal_length) {focal_length = p_focal_length;}
void Camera::set_aperture(float p_aperture) {aperture = p_aperture;}

glm::vec3 Camera::get_pos() {return pos;}
float Camera::get_yaw() {return yaw;}
float Camera::get_pitch() {return pitch;}
float Camera::get_fov() {return fov;}
float Camera::get_aspect() {return aspect;}
float Camera::get_focal_length() {return focal_length;}
float Camera::get_aperture() {return aperture;}

glm::vec3 Camera::calc_forward_dir() {
    return rot_mat * glm::vec4 {0.f, 0.f, -1.f, 1.f};
}

glm::vec3 Camera::calc_right_dir() {
    return rot_mat * glm::vec4 {1.f, 0.f, 0.f, 1.f};
}

glm::vec3 Camera::calc_up_dir() {
    return rot_mat * glm::vec4 {0.f, 1.f, 0.f, 1.f};
}

glm::vec3 Camera::calc_view_dir_at(glm::vec2 screen_coord) {
    glm::vec4 dir {
        screen_coord.x * abs_height * aspect,
        screen_coord.y * abs_height,
        -1.f,
        1.f
    };

    dir = glm::vec4 {math::normalize(dir.xyz()), dir.w};
    return rot_mat * dir;
}

Ray Camera::calc_ray_at(glm::vec2 screen_coord, RNG *rng) {
    glm::vec3 primary_dir {
        screen_coord.x * abs_height * aspect,
        screen_coord.y * abs_height,
        -1.f
    };
    primary_dir = math::normalize(primary_dir.xyz());
    glm::vec3 focal_point = pos + primary_dir * focal_length;

    // random dof offset
    std::uniform_real_distribution<float> distrib {0.f, 1.f};
    float r = aperture * std::sqrt(distrib(*rng));
    float theta = distrib(*rng) * 6.283185307f;
    glm::vec4 secondary_offset {
        r * cos(theta),
        r * sin(theta),
        0.f,
        1.f
    };
    secondary_offset = rot_mat * secondary_offset;
    glm::vec3 secondary_origin = pos + secondary_offset.xyz();

    glm::vec4 secondary_dir {
        glm::vec3 {focal_point - secondary_origin},
        1.f
    };
    secondary_dir = rot_mat * secondary_dir;

    return Ray {
        secondary_origin,
        math::normalize(secondary_dir.xyz()),
        TRC_CAMERA_RAY,
        0
    };
}

/* private */

void Camera::calc_rot_mat() {
    rot_mat = glm::mat4 {1.f};
    rot_mat = glm::rotate(rot_mat, glm::radians(yaw), glm::vec3(0.f, -1.f, 0.f));
    rot_mat = glm::rotate(rot_mat, glm::radians(pitch), glm::vec3(-1.f, 0.f, 0.f));
}

void Camera::calc_abs_height() {
    abs_height = tan(glm::radians(fov / 2.f));
}

} /* trc */
