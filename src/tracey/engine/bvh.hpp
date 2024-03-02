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

private:
    BVHnode bvh_root;

    void construct_BVH();
    void split_BVH_node(BVHnode &node);

    std::optional<Intersection> resolve_BVH_node(const BVHnode *node, Ray ray) const;

    Box shape_list_bounds(const std::list<Shape*> *list) const;
};

} /* trc */

#endif /* end of include guard: BVH_HPP */
