#include "point_light.hpp"

namespace trc {

PointLight(glm::vec3 p_pos, glm::vec3 p_color, float p_intensity, float p_radius)
    : Light(p_pos, p_color, p_intensity), radius(p_radius) {}

glm::vec3 PointLight::get_sample_point() {
    // TODO
}

} /* trc */
