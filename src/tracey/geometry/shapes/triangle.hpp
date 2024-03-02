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
    Triangle(glm::ivec3 p_data_index, Mesh *p_mesh, Shader *p_shader, Material *p_material);

    std::optional<Intersection> calc_ray_intersection(Ray ray) const;
    Box calc_bounding_box() const;
    glm::vec3 center() const;

    glm::ivec3 get_indices();
    Mesh *get_mesh();

private:
    glm::ivec3 data_index;

    Mesh *mesh;
};

} /* trc */

#endif /* end of include guard: TRIANGLE_HPP */
