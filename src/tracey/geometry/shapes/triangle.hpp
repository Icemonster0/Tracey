#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <memory>
#include <optional>

#include "../../../lib/glm.hpp"
#include "../../graphics/shader.hpp"
#include "../ray.hpp"
#include "../intersection.hpp"
#include "../shape.hpp"
#include "../mesh.hpp"

namespace trc {

class Triangle : public Shape {
public:
    Triangle(glm::ivec3 p_pos_i, glm::ivec3 p_normal_i, glm::ivec3 p_tex_i, Mesh *p_mesh, Shader *p_shader, Material *p_material);

    std::optional<Intersection> calc_ray_intersection(Ray ray) const;

    glm::ivec3 get_pos_i();
    glm::ivec3 get_normal_i();
    glm::ivec3 get_tex_i();
    Mesh *get_mesh();

private:
    glm::ivec3 pos_i;
    glm::ivec3 normal_i;
    glm::ivec3 tex_i;

    Mesh *mesh;
};

} /* trc */

#endif /* end of include guard: TRIANGLE_HPP */
