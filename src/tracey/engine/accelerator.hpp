#ifndef ACCELERATOR_HPP
#define ACCELERATOR_HPP

#include <list>
#include <memory>
#include <optional>

#include "../../lib/glm.hpp"
#include "../geometry/shape.hpp"
#include "../geometry/intersection.hpp"
#include "../geometry/ray.hpp"
#include "../geometry/box.hpp"
#include "scene.hpp"
#include "../util/random.hpp"

namespace trc {

class Accelerator {
public:
    Accelerator(Scene *source_scene);
    Accelerator();

    virtual std::optional<Intersection> calc_intersection(Ray ray) const;
    virtual LightSampleData calc_light_intersection(Ray ray) const;

    virtual glm::vec3 calc_light_influence(glm::vec3 shading_point, glm::vec3 normal, glm::vec3 view, float roughness, RNG *rng, float (*brdf)(glm::vec3, glm::vec3, glm::vec3, float)) const;

    glm::vec3 get_environment_light(Ray ray) const;

protected:
    std::list<Shape*> object_ptr_list;
    std::list<Light*> light_ptr_list;
    EnvironmentTexture *env_texture_ptr;

    float calc_light_occlusion(glm::vec3 shading_point, glm::vec3 source_point, float occlusion) const;

    std::optional<Intersection> calc_intersection_in_list(Ray ray, const std::list<Shape*> *list) const;
};

} /* trc */

#endif /* end of include guard: ACCELERATOR_HPP */
