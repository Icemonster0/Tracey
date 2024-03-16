#include "accelerator.hpp"

#include <limits>

#include "../util/math_util.hpp"

#include <unistd.h>

namespace trc {

// public

Accelerator::Accelerator(Scene *source_scene) {
    std::list<std::unique_ptr<Shape>> *source_object_list = source_scene->get_object_list();
    for (auto &obj : *source_object_list) {
        object_ptr_list.push_back(obj.get());
    }

    std::list<std::unique_ptr<Light>> *source_light_list = source_scene->get_light_list();
    for (auto &light : *source_light_list) {
        light_ptr_list.push_back(light.get());
    }

    env_texture_ptr = source_scene->get_environment();
}

Accelerator::Accelerator() {}

std::optional<Intersection> Accelerator::calc_intersection(Ray ray) const {
    return calc_intersection_in_list(ray, &object_ptr_list);
}

LightSampleData Accelerator::calc_light_intersection(Ray ray) const {
    float min_dist = std::numeric_limits<float>::infinity();
    LightSampleData min_isect {
        glm::vec3 {0.f},
        ray,
        std::numeric_limits<float>::infinity()
    };

    for (auto &light : light_ptr_list) {
        LightSampleData isect = light->calc_ray_intersection(ray);
        if (isect.light != glm::vec3(0.f) && isect.distance <= min_dist) {
            min_dist = isect.distance;
            min_isect = std::move(isect);
        }
    }

    return min_isect;
}

glm::vec3 Accelerator::calc_light_influence(glm::vec3 shading_point, glm::vec3 normal, glm::vec3 view, float roughness, RNG *rng, float (*brdf)(glm::vec3, glm::vec3, glm::vec3, float)) const {
    glm::vec3 light_color {0.f};

    for (auto &light : light_ptr_list) {
        LightSampleData light_data = light->sample(shading_point, rng);

        float cos_theta = glm::min(glm::dot(normal, light_data.shadow_ray.direction), 1.f);
        if (cos_theta < 0.f) continue;
        float occlusion = calc_light_occlusion(shading_point, light_data.shadow_ray.direction, light_data.distance, 0.f);
        light_color += light_data.light
                     * (1.f - occlusion)
                     * cos_theta
                     * brdf(light_data.shadow_ray.direction, view, normal, roughness);
    }

    return light_color;
}

float Accelerator::calc_light_occlusion(glm::vec3 shading_point, glm::vec3 direction, float distance, float occlusion) const {
    std::optional<Intersection> isect = calc_intersection(Ray {shading_point, direction, TRC_SHADOW_RAY});

    if (isect && isect.value().distance < distance) {
        occlusion += isect.value().material->alpha->sample(isect.value().tex_coord);
        if (occlusion < 1.f) {
            return calc_light_occlusion(isect.value().pos, direction, distance, occlusion);
        }
        else {
            return 1.f;
        }
    }
    else {
        return occlusion;
    }
}

glm::vec3 Accelerator::get_environment_light(Ray ray) const {
    return env_texture_ptr->sample(ray.direction);
}

// private

std::optional<Intersection> Accelerator::calc_intersection_in_list(Ray ray, const std::list<Shape*> *list) const {
    float min_dist = std::numeric_limits<float>::infinity();
    std::optional<Intersection> min_isect;

    for (auto &obj : *list) {
        std::optional<Intersection> isect = obj->calc_ray_intersection(ray);
        if (isect && isect.value().distance < min_dist) {
            min_dist = isect.value().distance;
            min_isect = std::move(isect);
        }
    }

    return min_isect;
}

Box Accelerator::shape_list_bounds(const std::list<Shape*> *list) const {
    Box bounds {};
    if (list->empty()) return bounds;

    auto it = list->begin();
    bounds = (*it)->calc_bounding_box();
    ++it;

    while (it != list->end()) {
        bounds.include((*it)->calc_bounding_box());
        ++it;
    }

    return bounds;
}

} /* trc */
