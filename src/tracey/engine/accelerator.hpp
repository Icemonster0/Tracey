#ifndef ACCELERATOR_HPP
#define ACCELERATOR_HPP

#include <list>
#include <memory>
#include <optional>

#include "../../lib/glm.hpp"
#include "../geometry/shape.hpp"
#include "../geometry/intersection.hpp"
#include "../geometry/ray.hpp"
#include "scene.hpp"
#include "../util/random.hpp"

namespace trc {

class Accelerator {
public:
    Accelerator(Scene *source_scene);
    Accelerator();

    std::optional<Intersection> calc_intersection(Ray ray) const;
    LightSampleData calc_light_intersection(Ray ray) const;
    glm::vec3 calc_light_influence(glm::vec3 shading_point, glm::vec3 normal, glm::vec3 view, float roughness, RNG *rng, float (*brdf)(glm::vec3, glm::vec3, glm::vec3, float)) const;

private:
    std::list<Shape*> object_ptr_list;
    std::list<Light*> light_ptr_list;
};

} /* trc */

#endif /* end of include guard: ACCELERATOR_HPP */
