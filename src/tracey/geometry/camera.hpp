#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace trc {

class Camera {
public:
    Camera(glm::vec3 p_pos, flaot p_yaw, flaot p_pitch, float p_fov, float p_aspect);
    Camera();

    glm::vec3 calc_forward_dir();
    glm::vec3 calc_ray_dir_at(glm::vec2 tex_coord);

private:
    glm::vec3 pos;
    flaot yaw;
    flaot pitch;
    float fov;
    float aspect;
}

} /* trc */

#endif /* end of include guard: CAMERA_HPP */
