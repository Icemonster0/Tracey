#include "triangle.hpp"

#include "../../util/math_util.hpp"

namespace trc {

Triangle::Triangle(glm::ivec3 p_data_index, Mesh *p_mesh, Shader *p_shader, Material *p_material)
    : Shape(p_shader, p_material), data_index(p_data_index), mesh(p_mesh) {}

std::optional<Intersection> Triangle::calc_ray_intersection(Ray ray) const {
    // fetch the three corners' coordinates
    glm::vec3 pos_a = mesh->get_pos(data_index.x);
    glm::vec3 pos_b = mesh->get_pos(data_index.y);
    glm::vec3 pos_c = mesh->get_pos(data_index.z);

    // transform to ray-space --------------------------------------------------

    // translate so that ray.origin is at the origin
    pos_a -= ray.origin;
    pos_b -= ray.origin;
    pos_c -= ray.origin;

    // permutate components to optimally orient the ray towards the negative z-axis
    glm::vec3 dir_sign = glm::sign(ray.direction);
    if (fabsf(ray.direction.x) > fabsf(ray.direction.y)) {
        if (fabsf(ray.direction.z) > fabsf(ray.direction.x)) { // z is greatest
            ray.direction *= -dir_sign.z;
            pos_a *= -dir_sign.z;
            pos_b *= -dir_sign.z;
            pos_c *= -dir_sign.z;
        } else { // x is greatest
            ray.direction = glm::vec3 {ray.direction.y, ray.direction.z, ray.direction.x} * -dir_sign.x;
            pos_a = glm::vec3 {pos_a.y, pos_a.z, pos_a.x} * -dir_sign.x;
            pos_b = glm::vec3 {pos_b.y, pos_b.z, pos_b.x} * -dir_sign.x;
            pos_c = glm::vec3 {pos_c.y, pos_c.z, pos_c.x} * -dir_sign.x;
        }
    } else if (fabsf(ray.direction.z) > fabsf(ray.direction.y)) { // z is greatest
        ray.direction *= -dir_sign.z;
        pos_a *= -dir_sign.z;
        pos_b *= -dir_sign.z;
        pos_c *= -dir_sign.z;
    } else { // y is greatest
        ray.direction = glm::vec3 {ray.direction.z, ray.direction.x, ray.direction.y} * -dir_sign.y;
        pos_a = glm::vec3 {pos_a.z, pos_a.x, pos_a.y} * -dir_sign.y;
        pos_b = glm::vec3 {pos_b.z, pos_b.x, pos_b.y} * -dir_sign.y;
        pos_c = glm::vec3 {pos_c.z, pos_c.x, pos_c.y} * -dir_sign.y;
    }

    // shear so that the ray is aligned with the z-axis
    float inv_ray_z = 1.f / ray.direction.z;
    float dx_dz = ray.direction.x * inv_ray_z;
    float dy_dz = ray.direction.y * inv_ray_z;
    glm::vec3 shear_offset_a {pos_a.z * dx_dz, pos_a.z * dy_dz, 0.f};
    glm::vec3 shear_offset_b {pos_b.z * dx_dz, pos_b.z * dy_dz, 0.f};
    glm::vec3 shear_offset_c {pos_c.z * dx_dz, pos_c.z * dy_dz, 0.f};
    pos_a -= shear_offset_a;
    pos_b -= shear_offset_b;
    pos_c -= shear_offset_c;

    // check intersection ------------------------------------------------------

    // calculate barycentric coordinates
    float area = math::triangle_signed_area(pos_a.xy(), pos_b.xy(), pos_c.xy());
    float area_a = math::triangle_signed_area(glm::vec3 {0.f}, pos_b.xy(), pos_c.xy());
    float area_b = math::triangle_signed_area(pos_a.xy(), glm::vec3 {0.f}, pos_c.xy());
    // float area_c = math::triangle_signed_area(pos_a.xy(), pos_b.xy(), glm::vec3 {0.f});

    float area_inv = 1.f / area;
    float b_a = area_a * area_inv;
    float b_b = area_b * area_inv;
    // float b_c = area_c * area_inv;
    float b_c = 1.f - b_a - b_b;

    if (b_a < 0 || b_b < 0 || b_c < 0) // no intersection
        return std::optional<Intersection>();

    // claculate intersection parameters ---------------------------------------
    glm::vec3 pos = b_a * mesh->get_pos(data_index.x)
                  + b_b * mesh->get_pos(data_index.y)
                  + b_c * mesh->get_pos(data_index.z);

    float distance = glm::length(pos - ray.origin);

    if (distance < TRC_RAY_CLIP_EPSILON || (b_a*pos_a.z + b_b*pos_b.z + b_c*pos_c.z) > 0) // too close or behind the ray origin
        return std::optional<Intersection>();

    glm::vec3 normal = b_a * mesh->get_normal(data_index.x)
                     + b_b * mesh->get_normal(data_index.y)
                     + b_c * mesh->get_normal(data_index.z);
    glm::vec3 tan = b_a * mesh->get_tan(data_index.x)
                  + b_b * mesh->get_tan(data_index.y)
                  + b_c * mesh->get_tan(data_index.z);
    glm::vec3 bitan = b_a * mesh->get_bitan(data_index.x)
                    + b_b * mesh->get_bitan(data_index.y)
                    + b_c * mesh->get_bitan(data_index.z);
    glm::vec2 tex = b_a * mesh->get_tex(data_index.x)
                  + b_b * mesh->get_tex(data_index.y)
                  + b_c * mesh->get_tex(data_index.z);

    return std::make_optional<Intersection> (
        pos,
        normal,
        tan,
        bitan,
        tex,
        material,
        distance,
        shader
    );
}

Box Triangle::calc_bounding_box() const {
    float min_x = std::min(std::min(mesh->get_pos(data_index.x).x, mesh->get_pos(data_index.y).x), mesh->get_pos(data_index.z).x);
    float max_x = std::max(std::max(mesh->get_pos(data_index.x).x, mesh->get_pos(data_index.y).x), mesh->get_pos(data_index.z).x);
    float min_y = std::min(std::min(mesh->get_pos(data_index.x).y, mesh->get_pos(data_index.y).y), mesh->get_pos(data_index.z).y);
    float max_y = std::max(std::max(mesh->get_pos(data_index.x).y, mesh->get_pos(data_index.y).y), mesh->get_pos(data_index.z).y);
    float min_z = std::min(std::min(mesh->get_pos(data_index.x).z, mesh->get_pos(data_index.y).z), mesh->get_pos(data_index.z).z);
    float max_z = std::max(std::max(mesh->get_pos(data_index.x).z, mesh->get_pos(data_index.y).z), mesh->get_pos(data_index.z).z);

    return Box {
        glm::vec3 {min_x, min_y, min_z},
        glm::vec3 {max_x, max_y, max_z}
    };
}

glm::vec3 Triangle::center() const {
    // return mesh->get_pos(data_index.z);
    return (
        mesh->get_pos(data_index.x) +
        mesh->get_pos(data_index.y) +
        mesh->get_pos(data_index.z)
    ) / 3.f;
}

glm::ivec3 Triangle::get_indices() {
    return data_index;
}

Mesh *Triangle::get_mesh() {
    return mesh;
}

} /* trc */
