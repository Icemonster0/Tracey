#include "accelerator.hpp"

#include <limits>

namespace trc {

Accelerator::Accelerator(Scene *source_scene) {
    std::list<std::unique_ptr<Shape>> *source_object_list = source_scene->get_object_list();
    for (auto &obj : *source_object_list) {
        object_ptr_list.push_back(obj.get());
    }

    std::list<std::unique_ptr<Light>> *source_light_list = source_scene->get_light_list();
    for (auto &light : *source_light_list) {
        light_ptr_list.push_back(light.get());
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

glm::vec3 Accelerator::calc_light_influence(glm::vec3 shading_point, glm::vec3 normal, RNG *rng) const {
    glm::vec3 light_color {0.f};

    for (auto &light : light_ptr_list) {
        LightSampleData light_data = light->sample(shading_point, rng);
        std::optional<Intersection> isect = calc_intersection(light_data.shadow_ray);
        if (isect && isect.value().distance < light_data.distance) {
            continue;
        }
        else light_color += light_data.light * glm::clamp(glm::dot(normal, light_data.shadow_ray.direction), 0.f, 1.f);
    }

    return light_color;
}

} /* trc */
