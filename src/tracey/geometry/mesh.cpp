#include "mesh.hpp"

namespace trc {

Mesh::Mesh(std::vector<glm::vec3> p_pos,
           std::vector<glm::vec3> p_normal,
           std::vector<glm::vec2> p_tex)
    : pos(p_pos), normal(p_normal), tex(p_tex) {}

glm::vec3 Mesh::get_pos(int index) {
    return pos[index];
}

glm::vec3 Mesh::get_normal(int index) {
    return normal[index];
}

glm::vec2 Mesh::get_tex(int index) {
    return tex[index];
}

} /* trc */
