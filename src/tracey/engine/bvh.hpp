#ifndef BVH_HPP
#define BVH_HPP

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

struct BVHnode {
    Box bounding_box;
    std::list<BVHnode> children;
    std::list<Shape*> leaves;
};

class BVH : public Accelerator {
public:
    BVH(Scene *source_scene);
    BVH();

    std::optional<Intersection> calc_intersection(Ray ray) const;

    unsigned long get_memory_usage() const;

private:
    BVHnode bvh_root;

    void construct_BVH();
    void split_BVH_node(BVHnode &node);

    std::optional<Intersection> resolve_BVH_node(const BVHnode *node, Ray ray) const;

    unsigned long count_mem_usage_of_node(const BVHnode &node) const;
};

} /* trc */

#endif /* end of include guard: BVH_HPP */
