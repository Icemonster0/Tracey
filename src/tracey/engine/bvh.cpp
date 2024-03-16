#include "bvh.hpp"

#include <limits>

#include "../util/math_util.hpp"

#include <unistd.h>

namespace trc {

// public

BVH::BVH(Scene *source_scene) : Accelerator(source_scene) {
    construct_BVH();
}

BVH::BVH() {}

std::optional<Intersection> BVH::calc_intersection(Ray ray) const {
    return resolve_BVH_node(&bvh_root, ray);
}

// private

void BVH::construct_BVH() {
    printf("Building BVH ...\n");
    bvh_root.leaves = object_ptr_list;
    bvh_root.bounding_box = shape_list_bounds(&bvh_root.leaves);
    split_BVH_node(bvh_root);
}

void BVH::split_BVH_node(BVHnode &node) {
    // first, check whether there are only zero to two leaves
    auto leaf_checker_iter = node.leaves.begin();
    for (int i = 0; i < 3; ++i) {
        if (leaf_checker_iter == node.leaves.end())
            return;
        ++leaf_checker_iter;
    }

    // if there are more, split the leaves between two child nodes

    BVHnode l_child, r_child;
    unsigned char axis = node.bounding_box.max_dimension();
    glm::vec3 node_center = node.bounding_box.center();

    for (int i = 0; i < 3; ++i) {
        for (Shape *leaf : node.leaves) {
            bool id_bit;
            switch (axis) {
                case 0: id_bit = (leaf->center().x > node_center.x); break;
                case 1: id_bit = (leaf->center().y > node_center.y); break;
                case 2: id_bit = (leaf->center().z > node_center.z); break;
                default: printf("There is a bug in accelerator.cpp: split_BVH_node(): axis must be 0, 1 or 2\n"); break;
            }

            if (id_bit) {
                r_child.leaves.push_back(leaf);
            } else {
                l_child.leaves.push_back(leaf);
            }
        }

        if (!r_child.leaves.empty() && !l_child.leaves.empty())
            break;

        // in case not both children have leaves, try again
        r_child.leaves.clear();
        l_child.leaves.clear();
        axis = (axis + 1) % 3;
    }

    // after three attempts, if both children still don't have leaves, give up
    if (r_child.leaves.empty() || l_child.leaves.empty())
        return;

    node.leaves.clear();

    if (!l_child.leaves.empty()) {
        l_child.bounding_box = shape_list_bounds(&l_child.leaves);
        split_BVH_node(l_child);
        node.children.push_back(std::move(l_child));
    }

    if (!r_child.leaves.empty()) {
        r_child.bounding_box = shape_list_bounds(&r_child.leaves);
        split_BVH_node(r_child);
        node.children.push_back(std::move(r_child));
    }
}

std::optional<Intersection> BVH::resolve_BVH_node(const BVHnode *node, Ray ray) const {
    if (node->bounding_box.intersect_ray(ray)) {
        if (!node->leaves.empty()) {
            return calc_intersection_in_list(ray, &node->leaves);
        }
        else {
            float min_dist = std::numeric_limits<float>::infinity();
            std::optional<Intersection> min_isect;
            for (auto &child : node->children) {
                std::optional<Intersection> isect = resolve_BVH_node(&child, ray);
                if (isect && isect.value().distance < min_dist) {
                    min_dist = isect.value().distance;
                    min_isect = std::move(isect);
                }
            }
            return min_isect;
        }
    }
    else {
        return std::optional<Intersection>();
    }
}

} /* trc */
