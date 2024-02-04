#include "intersection.hpp"

namespace trc {

Intersection::Intersection(glm::vec3 p_pos, glm::vec3 p_normal, glm::vec2 p_tex_coord, float p_distance, Shader *p_shader)
    : pos(p_pos), normal(p_normal), tex_coord(p_tex_coord), distance(p_distance), shader(p_shader) {}

} /* trc */
