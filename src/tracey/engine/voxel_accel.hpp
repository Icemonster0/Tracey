#ifndef VOXEL_ACCEL_HPP
#define VOXEL_ACCEL_HPP

#include <list>
#include <memory>
#include <optional>

#include "../../lib/glm.hpp"
#include "accelerator.hpp"
#include "../geometry/shape.hpp"
#include "../geometry/intersection.hpp"
#include "../geometry/ray.hpp"
#include "../geometry/box.hpp"
#include "scene.hpp"
#include "../util/random.hpp"

namespace trc {

class VoxelAccel : public Accelerator {
public:
    VoxelAccel(Scene *source_scene, float p_voxel_size);
    VoxelAccel();

    std::optional<Intersection> calc_intersection(Ray ray) const;

    unsigned long get_memory_usage() const;

private:
    std::vector<std::vector<std::vector<std::list<Shape*>>>> grid;
    glm::mat4 world_to_grid;
    glm::ivec3 grid_size;
    float voxel_size;

    void populate_grid(float p_voxel_size);
};

} /* trc */

#endif /* end of include guard: VOXEL_ACCEL_HPP */
