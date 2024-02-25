#include "intersection.hpp"

namespace trc {

Intersection::Intersection(glm::vec3 p_pos, glm::vec3 p_normal, glm::vec3 p_tan, glm::vec3 p_bitan, glm::vec2 p_tex_coord, Material *p_material, float p_distance, Shader *p_shader)
    : pos(p_pos), normal(p_normal), tan(p_tan), bitan(p_bitan), tex_coord(p_tex_coord), material(p_material), distance(p_distance), shader(p_shader) {}

} /* trc */
