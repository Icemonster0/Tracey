#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "../../lib/glm.hpp"

namespace trc {

class Mesh {
public:
    Mesh(std::vector<glm::vec3> p_pos,
         std::vector<glm::vec3> p_normal,
         std::vector<glm::vec2> p_tex);

    glm::vec3 get_pos(int index);
    glm::vec3 get_normal(int index);
    glm::vec2 get_tex(int index);

private:
    std::vector<glm::vec3> pos;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec2> tex;
};

} /* trc */

#endif /* end of include guard: MESH_HPP */
