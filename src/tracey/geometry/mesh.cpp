#include "mesh.hpp"

namespace trc {

Mesh::Mesh(std::vector<glm::vec3> p_pos,
           std::vector<glm::vec3> p_normal,
           std::vector<glm::vec3> p_tan,
           std::vector<glm::vec3> p_bitan,
           std::vector<glm::vec2> p_tex)
    : pos(p_pos), normal(p_normal), tan(p_tan), bitan(p_bitan), tex(p_tex) {}

glm::vec3 Mesh::get_pos(int index) { return pos[index]; }
glm::vec3 Mesh::get_normal(int index) { return normal[index]; }
glm::vec3 Mesh::get_tan(int index) { return tan[index]; }
glm::vec3 Mesh::get_bitan(int index) { return bitan[index]; }
glm::vec2 Mesh::get_tex(int index) { return tex[index]; }

int Mesh::get_pos_size() { return pos.size(); }
int Mesh::get_normal_size() { return normal.size(); }
int Mesh::get_tan_size() { return tan.size(); }
int Mesh::get_bitan_size() { return bitan.size(); }
int Mesh::get_tex_size() { return tex.size(); }

} /* trc */
