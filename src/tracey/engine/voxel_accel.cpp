#include "voxel_accel.hpp"

#include <limits>
#include <stdexcept>
#include <unistd.h>

#include "../util/math_util.hpp"

namespace trc {

// public

VoxelAccel::VoxelAccel(Scene *source_scene, float p_voxel_size) : Accelerator(source_scene) {
    populate_grid(p_voxel_size);
}

VoxelAccel::VoxelAccel() {}

std::optional<Intersection> VoxelAccel::calc_intersection(Ray ray) const {
    // avoid infinite loop
    if (glm::all(glm::equal(ray.direction, glm::vec3(0.f))))
        return std::optional<Intersection>();
    ray.direction = math::normalize(ray.direction);

    glm::vec3 tdelta;
    glm::vec3 inv_dir = 1.f / ray.direction;
    if (ray.direction.x == 0.f) {
        tdelta.x = std::numeric_limits<float>::infinity();
    } else {
        float dy_dx = ray.direction.y * inv_dir.x;
        float dz_dx = ray.direction.z * inv_dir.x;
        tdelta.x = sqrt(dy_dx*dy_dx + dz_dx*dz_dx + 1.f);
    }
    if (ray.direction.y == 0.f) {
        tdelta.y = std::numeric_limits<float>::infinity();
    } else {
        float dx_dy = ray.direction.x * inv_dir.y;
        float dz_dy = ray.direction.z * inv_dir.y;
        tdelta.y = sqrt(dx_dy*dx_dy + dz_dy*dz_dy + 1.f);
    }
    if (ray.direction.z == 0.f) {
        tdelta.z = std::numeric_limits<float>::infinity();
    } else {
        float dx_dz = ray.direction.x * inv_dir.z;
        float dy_dz = ray.direction.y * inv_dir.z;
        tdelta.z = sqrt(dx_dz*dx_dz + dy_dz*dy_dz + 1.f);
    }

    glm::vec3 start = (world_to_grid * glm::vec4 {ray.origin, 1.f}).xyz();

    // handle ray origin outside of grid
    float initial_t = 0.f;
    if (!glm::all(glm::greaterThanEqual(start, glm::vec3(0.f)) && glm::lessThan(start, glm::vec3(grid_size)))) {
        glm::vec2 t = math::intersect_aabb(
            Ray {start, ray.direction},
            glm::vec3(0.f),
            glm::vec3(grid_size)-voxel_size*0.1f
        );
        if (t.x > t.y) return std::optional<Intersection>();
        initial_t = t.x;
        start += ray.direction * initial_t;
    }

    glm::ivec3 voxel = start;
    voxel = glm::clamp(voxel, glm::ivec3(0), grid_size-1);

    glm::ivec3 step = glm::sign(ray.direction);

    glm::vec3 tmax {
        (step.x == -1 ? glm::fract(start.x) : 1.f - glm::fract(start.x)) * tdelta.x,
        (step.y == -1 ? glm::fract(start.y) : 1.f - glm::fract(start.y)) * tdelta.y,
        (step.z == -1 ? glm::fract(start.z) : 1.f - glm::fract(start.z)) * tdelta.z
    };

#   define TRAVERSE_GRID(a) \
        if (!grid[voxel.x][voxel.y][voxel.z].empty()) { \
            std::optional<Intersection> isect = calc_intersection_in_list(ray, &grid[voxel.x][voxel.y][voxel.z]); \
            if (isect && isect.value().distance < (tmax.a + initial_t + voxel_size*0.1f) * voxel_size) { \
                return isect; \
            } \
        } \
        voxel.a += step.a; \
        if(voxel.a < 0 || voxel.a >= grid_size.a) { \
            return std::optional<Intersection>(); \
        } \
        tmax.a += tdelta.a;

    // grid traversal
    while (true) {
        if(tmax.x < tmax.y) {
            if(tmax.x < tmax.z) {
                TRAVERSE_GRID(x);
            } else {
                TRAVERSE_GRID(z);
            }
        } else {
            if(tmax.y < tmax.z) {
                TRAVERSE_GRID(y);
            } else {
                TRAVERSE_GRID(z);
            }
        }
    }

#   undef TRAVERSE_GRID
}

// private

void VoxelAccel::populate_grid(float p_voxel_size) {
    printf("Building Voxel Accelerator ...\n");

    voxel_size = p_voxel_size;

    Box bounding_box = shape_list_bounds(&object_ptr_list);
    glm::vec3 volume_size = bounding_box.pmax - bounding_box.pmin + voxel_size*0.1f;
    grid_size = glm::ceil(volume_size / voxel_size);

    world_to_grid = glm::mat4 {1.f};
    world_to_grid = glm::scale(world_to_grid, glm::vec3 {1.f / voxel_size});
    world_to_grid = glm::translate(world_to_grid, -bounding_box.pmin);

    try {
        grid = std::vector<std::vector<std::vector<std::list<Shape*>>>> {
            (long unsigned int)grid_size.x,
            std::vector<std::vector<std::list<Shape*>>> {
                (long unsigned int)grid_size.y,
                std::vector<std::list<Shape*>> {
                    (long unsigned int)grid_size.z,
                    std::list<Shape*> {}
                }
            }
        };
    } catch (const std::length_error& e) {
        // object with infinite (or too big) extent detected
        printf("Floor planes are not supported in VOXEL mode (see --accelerator)\n");
        exit(2);
    }

    for (Shape *obj : object_ptr_list) {
        Box obj_box = obj->calc_bounding_box();
        glm::ivec3 voxel_min = glm::floor(world_to_grid * glm::vec4 {obj_box.pmin, 1.f}).xyz();
        glm::ivec3 voxel_max = glm::floor(world_to_grid * glm::vec4 {obj_box.pmax, 1.f}).xyz();

        for (int x = voxel_min.x; x <= voxel_max.x; ++x) {
            for (int y = voxel_min.y; y <= voxel_max.y; ++y) {
                for (int z = voxel_min.z; z <= voxel_max.z; ++z) {
                    grid[x][y][z].push_back(obj);
                }
            }
        }
    }
}

} /* trc */
