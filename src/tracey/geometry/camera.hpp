#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../../glm.hpp"

namespace trc {

class Camera {
public:
    Camera(glm::vec3 p_pos, float p_yaw, float p_pitch, float p_fov, float p_aspect);
    Camera();

    void set_pos(glm::vec3 p_pos);
    void set_yaw(float p_yaw);
    void set_pitch(float p_pitch);
    void set_fov(float p_fov);
    void set_aspect(float p_aspect);

    glm::vec3 get_pos();
    float get_yaw();
    float get_pitch();
    float get_fov();
    float get_aspect();

    glm::vec3 calc_forward_dir();
    glm::vec3 calc_right_dir();
    glm::vec3 calc_up_dir();
    glm::vec3 calc_ray_dir_at(glm::vec2 tex_coord);

private:
    glm::vec3 pos;
    float yaw;
    float pitch;
    float fov;
    float aspect;

    glm::mat4 rot_mat;
    float abs_height;

    void calc_rot_mat();
    void calc_abs_height();
};

} /* trc */

#endif /* end of include guard: CAMERA_HPP */
