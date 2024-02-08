#include "accelerator.hpp"

#include <limits>

namespace trc {

Accelerator::Accelerator(Scene *source_scene) {
    std::list<std::unique_ptr<Shape>> *source_object_list = source_scene->get_object_list();
    for (auto &obj : *source_object_list) {
        object_ptr_list.push_back(obj.get());
    }
}

Accelerator::Accelerator() {}

std::optional<Intersection> Accelerator::calc_intersection(Ray ray) const {
    float min_dist = std::numeric_limits<float>::infinity();
    std::optional<Intersection> min_isect;

    for (auto &obj : object_ptr_list) {
        std::optional<Intersection> isect = obj->calc_ray_intersection(ray);
        if (isect && isect.value().distance < min_dist) {
            min_dist = isect.value().distance;
            min_isect = isect;
        }
    }

    return min_isect;
}

} /* trc */
