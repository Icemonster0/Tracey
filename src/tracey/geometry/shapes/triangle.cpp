#include "triangle.hpp"

#include "../../util/math_util.hpp"

namespace trc {

Triangle::Triangle(glm::ivec3 p_pos_i, glm::ivec3 p_normal_i, glm::ivec3 p_tan_i, glm::ivec3 p_bitan_i, glm::ivec3 p_tex_i, Mesh *p_mesh, Shader *p_shader, Material *p_material)
    : Shape(p_shader, p_material), pos_i(p_pos_i), normal_i(p_normal_i), tan_i(p_tan_i), bitan_i(p_bitan_i), tex_i(p_tex_i), mesh(p_mesh) {}

std::optional<Intersection> Triangle::calc_ray_intersection(Ray ray) const {
    // fetch the three corners' coordinates
    glm::vec3 pos_a = mesh->get_pos(pos_i.x);
    glm::vec3 pos_b = mesh->get_pos(pos_i.y);
    glm::vec3 pos_c = mesh->get_pos(pos_i.z);

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
    glm::vec3 pos = b_a * mesh->get_pos(pos_i.x)
                  + b_b * mesh->get_pos(pos_i.y)
                  + b_c * mesh->get_pos(pos_i.z);

    float distance = glm::length(pos - ray.origin);

    if (distance < TRC_RAY_CLIP_EPSILON || (b_a*pos_a.z + b_b*pos_b.z + b_c*pos_c.z) > 0) // too close or behind the ray origin
        return std::optional<Intersection>();

    glm::vec3 normal = b_a * mesh->get_normal(normal_i.x)
                     + b_b * mesh->get_normal(normal_i.y)
                     + b_c * mesh->get_normal(normal_i.z);
    glm::vec3 tan = b_a * mesh->get_tan(tan_i.x)
                  + b_b * mesh->get_tan(tan_i.y)
                  + b_c * mesh->get_tan(tan_i.z);
    glm::vec3 bitan = b_a * mesh->get_bitan(bitan_i.x)
                    + b_b * mesh->get_bitan(bitan_i.y)
                    + b_c * mesh->get_bitan(bitan_i.z);
    glm::vec2 tex = b_a * mesh->get_tex(tex_i.x)
                  + b_b * mesh->get_tex(tex_i.y)
                  + b_c * mesh->get_tex(tex_i.z);

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

glm::ivec3 Triangle::get_pos_i() {
    return pos_i;
}

glm::ivec3 Triangle::get_normal_i() {
    return normal_i;
}

glm::ivec3 Triangle::get_tan_i() {
    return tan_i;
}

glm::ivec3 Triangle::get_bitan_i() {
    return bitan_i;
}

glm::ivec3 Triangle::get_tex_i() {
    return tex_i;
}

Mesh *Triangle::get_mesh() {
    return mesh;
}

} /* trc */
